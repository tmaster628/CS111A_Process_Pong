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
    // Pipe declarations.
    int p1_net[2], net_p1[2],p2_net[2], net_p2[2];
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
    float factor = player_skill * .3 + prev_shot_quality* .5 + random*.2;
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