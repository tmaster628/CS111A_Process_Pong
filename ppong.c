/*
 * Multiprocessing Tennis (Process Pong)
 * Author: Trip Master
 * ------------------
 * This program is an exercize in multiprocessing. Students will create two processes
 * and send data back and forth through the parent process via pipes. For a detailed spec,
 * check out https://github.com/tmaster628/CS111A_Process_Pong
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>

float player_hit (float player_skill, float prev_shot_difficulty);
bool over_net (float player_skill, float shot_quality);

int main() {
    srand(time(NULL));
    const float player1_skill = 0.8;
    const float player2_skill = 0.8;

    
    /* Start Code Here */

    // Step 1: Create pipes.
    int p1_net[2], net_p1[2],p2_net[2], net_p2[2];   
    pipe(p1_net);
    pipe(net_p1);
    pipe(p2_net);
    pipe(net_p2);

    // Fork off player 1.
    pid_t player1_pid = fork();
    if (player1_pid == 0) {
        // Close unused fds. Necessary for correctness.
        
        /*TODO: Close all FD's that aren't relevant to player 1. */
		// Begin Section

		// End section

        float prev_diff =  0.0;
        float shot_outcome = player_hit(player1_skill, prev_diff);
        while(true) {
            // Simulate player 1 'hitting' the ball.
            char buf[4];
            gcvt(shot_outcome, 2, buf);
            printf("Player 1 hits the ball.\n");
				    
			/*TODO: write() the buffer to the pipe associated with the net. */
			// Begin Section

			// End section
            
            // Now we wait for player 2 to hit back to us.
            char buf2[4];
           	/*TODO: read() the value of the shot coming from the net into buf2. */
			// Begin Section

			// End section
            prev_diff = atof(buf2);
            if (prev_diff > 1) {
                // This is a signal that Player 2 lost the point. Nothing more to do.
                break;
            }

            shot_outcome = player_hit(player1_skill, prev_diff);
            if (shot_outcome < 0.5) {
                // In this case, we 'miss' the ball. End the point.
                printf("Player 1 misses the ball.\n");
                printf("Player 2 wins the point!\n");

                // End the point.
                char term[4]; 
                gcvt(shot_outcome, 2, term);
                
                /*TODO: write() the TERM buf to the net. */
				// Begin Section

				// End section
                break;
            }
        }

        /*TODO: Close remaining FD's */
		// Begin Section

		// End section
        return 0;
    }

    // Player 2's turn.
    pid_t player2_pid = fork();
    if (player2_pid == 0) {

        /*TODO: Close all FD's that aren't relevant to player 1. */
		// Begin Section

		// End section

        float shot_outcome = 0.0;
        while(true) {
            char buf[4];
            /*TODO: read() the value of the shot coming from the net into buf. */
			// Begin Section

			// End section
            float prev_dif = atof(buf);
            if (prev_dif > 1) {
                // Signifies Player 1 ended the point.
                break;
            }
            // Compute shot outcome for P2.
            shot_outcome = player_hit(player2_skill, prev_dif);
            if (shot_outcome < 0.5) {
                printf("Player 2 misses the ball.\n");
                printf("Player 1 wins the point!\n");
                shot_outcome = 2.0;

                char term[4]; 
                gcvt(shot_outcome, 2, term);
                /*TODO: write() the TERM buf to the net. */
				// Begin Section

				// End section
                break;
            }
            char buf2[4];
            gcvt(shot_outcome, 2, buf2);
            printf("Player 2 hits the ball.\n");
			/*TODO: write() the buffer to the pipe associated with the net. */
			// Begin Section

			// End section
        }

		    /*TODO: Close remaining FD's */
			// Begin Section

			// End section
            return 0;
    } 


    /*TODO: Close remaining FD's */
	// Begin Section

	// End section
    
    float shot_outcome = 0.0;
    while(true) {
        char buf_p1[4];
        /*TODO: Read the data written from P1 into buf_p1. */
		// Begin Section

		// End section
        shot_outcome = atof(buf_p1);
        if (shot_outcome > 1) break; // Means Player 1 missed the ball.
        if (over_net(player1_skill, shot_outcome)) {
        
		    /*TODO: Write the calculated result (buf_p1) to P2. */
			// Begin Section

			// End section        

            // Wait for P2 to hit to us...
            char buf_p2[4];
            
            /*TODO: Read the data from P2's hit into buf_p2. */
			// Begin Section

			// End section
            float shot_outcome2 = atof(buf_p2);
            if (shot_outcome2 > 1) break; // Means Player2 missed the ball.
            if (over_net(player2_skill, shot_outcome2)) {
				/*TODO: Write the resulting shot (buf_p2) to player 1. */
				// Begin Section

				// End section
            } else {
                printf("Player 2 hits the net.\n");
                printf("Player 1 wins the point!\n");
                break;
            }
        } else {
            printf("Player 1 hits the net.\n");
            printf("Player 2 wins the point!\n");
            break;
        }
    }

    // If we've gotten here, it means that The Net rejected a ball. Therefore
    // we need to write a sentinel value to both children so that they know
    // to terminate.
    char term[4];
    gcvt(2.0, 2, term);
    
    // We'll do this for you -- this WRITE ends the point.
    write(net_p1[1], term, sizeof(term));
    write(net_p2[1], term, sizeof(term));

	/*TODO: Close remaining FD's */
	// Begin Section

	// End section



    while (waitpid(-1, NULL, 0) != -1); // Nothing, just reap both players.
    return 0;
}


/**
 * player_hit
 * ----------
 * Given two float values between 0.0 and 1.0, returns the quality of a player's shot. Don't
 * worry about how this works.
*/
float player_hit (float player_skill, float prev_shot_quality) {
    float random = (float)rand()/RAND_MAX;
    float factor = player_skill * .3 + (1-prev_shot_quality) * .5 + random*.2;
    return factor;
}

/**
 * over_net
 * --------
 * Given the quality of a shot and the player's skill (numbers between 0.0 and 1.0), determines whether
 * a ball will go over the net.
 * 
 * Why the added complexity? Because it promotes ~learning~
*/
bool over_net (float player_skill, float shot_quality) {
    float random = (float)rand()/RAND_MAX;
    float factor = shot_quality * .6 + player_skill*.1 + random*.3; // Very sophisticated stuff here.
    return factor > 0.5;
}
