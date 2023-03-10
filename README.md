# Multiprocessing Tennis (Process Pong!)

Tennis (or tennes, as I affectionately spelled it in my youth) is the 4<sup>th</sup> most popular sport in the world, according to Wikipedia. 

Nifty. Now that I've introduced the concept, let's get on to the problem at hand.


## Problem Overview
Your task is to simulate a (simplified) outcome of a tennis point. Don't worry about the finer facets of the game: here are a few sample program outputs:

```
Player 1 hits the ball.
Player 2 hits the ball.
Player 2 hits the net.
Player 1 wins the point!
---
Player 1 hits the ball.
Player 2 misses the ball.
Player 1 wins the point!
---
```

To implement this,there are **three** major players at hand: Player 1, Player 2, and The Net. You will fork two processes to represent Player1 and Player 2. The parent process will be The Net. Next, let's discuss the structure of a point

## Point Structure
You wll need to implement the following. Consider a 'turn' to be the work done by one of the 3 processes.
### Player 1 turn
When Player 1 has the ball, there are *only* 2 things that can happen:
1. Player 1 can *hit* the ball towards Player 2.
2. Player 1 can *miss* the ball, resulting in a point for Player 2.

For the sake of simplicity, let's assume that a ball out of bounds counts as a miss.
To determine the outcome of a player's hit, We need 2 pieces of data: 
1. The float `player1_skill` as the probability (between 0.0 and 1.0) that the player will successfully hit the ball, independent of other conditions.
2. The float `previous_shot_quality`, which measures how difficult the previous shot was (0.0 - easiest to 1.0 - impossible). For example, if Player 2 hits a fantastic shot, player 1 will have a difficult time returning it successfully, regardless of `player1_skill`. For the first turn, you may assume that `previous_shot_quality` is 0.0.

Use these as the arguments to the `player_hit()` function, which returns a float representing the quality of Player 1's hit. For example:
```
float player1_hit_quality = player_hit (player1_skill, previous_shot_quality);
```

Next is where things get fun. You will `write` the `player1_hit_quality` value to the parent (Net) via a pipe. **Note** that you must also write the result of a *miss* to the parent (Net) in order to properly terminate your program. Think about ways you might be able to differentiate a hit or a miss with the values you `write`. After that, you'll need to wait for the opponent to hit. Recall that read() hangs until the correct amount of data is read (see example code below for specifics).

### The Net turn
To make things even more fun, The Net can reject balls hit from either player. The Net will reject a ball that is *successfully* hit from either player if the function `over_net()` returns false. This function takes in 2 parameters, the `skill` float of the player who hit the ball last, as well as the hit quality. If `over_net()` returns True, `write` the value of the hit quality to Player 2. If it returns False, the point is over, and the receiving player wins the point. **Note** that if The Net rejects a ball, both players must be written to in order to ensure that the game ends.

### Player 2 turn
Player 2's turn will look very similar to Player 1's turn. They will call `player_hit` with `player2_skill` and the quality of the previous shot (which is read from a pipe connected to The Net). You'll then write the result of Player 2's hit back to The Net, and the game continues!

## Program Design
There are a number of things to consider when designing this program:
* You will need to figure out where to put your print statements. So long as you're careful, you should have some flexibility here.
* Although the point can be "lost" when a player misses the ball, the point can only be ended by the parent process (The Net). Keep than in mind. You will probably need to send sentinel values between Parent / Children or vice-versa to ensure correctness in all cases.
* Make sure that all file descriptors *as soon as possible* (i.e, think about which FD's a process will never use). If your program is not completing, it's almost definitely because some FD's in any of the processes was left open. Similarly, ensure that both Player processes are reaped upon completion.
* Becuase sizing the buffer for read / write and string / float conversion can be stressful, here's some useful code to read a number like "0.5" into a buffer and convert it into a float:
```       
char buf[4]; // i.e 0.4 takes up 4 bytes.
read(fd, buf, sizeof(buf));
float float_version_of_buf = atof(buf);
```
* Now, here's a code snippet to convert a float like "0.5" into a char buffer and write it:
```
float some_float = 0.5;
char buf[4];
gcvt(some_float, 2, buf); // Converts float to string with 2 significant figures.
write(fd, buf, sizeof(buf));
```
So long as you use the right file descriptors, you should be able to use these code snippets exactly.

## Get the Starter Code
To clone this repository, log into myth, navigate to where you want the project to live, and then type

`git clone https://github.com/tmaster628/CS111A_Process_Pong.git`

From there, `cd` into the repo and type `make`. 



---
*Side note: This problem is more difficult than an exam problem, but a little less difficult than an assignment problem. Doing it in a class period would be very ambitious, but it's a great problem to solidify your understanding of Processes, FD's and Read/Write. If you have some time before an exam, I'd recommend giving this a try and checking out my reference solution.*
