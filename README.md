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
Player 1 hits the ball out of bounds.
Player 2 wins the point!
```

To implement this,there are **three** major players at hand: Player 1, Player 2, and The Net. You will fork two processes to represent Player1 and Player 2. The parent process will be The Net. Next, let's discuss the structure of a point

## Point Structure
In a loop, you'll want to implement the following:
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

Next is where things get fun. You will `write` the `player1_hit_quality` value to the parent (Net) via a pipe. **Note** that you must also write the result of a miss to the parent (Net) in order to properly terminate your program. Think about ways you might be able to differentiate a hit or a miss with the values you `write`. After that, you'll need to wait for the opponent to hit.

### The Net turn
To make things even more fun, The Net can reject balls hit from either player. The Net will reject a ball that is *successfully* hit from either player if the function `over_net()` returns false. This function takes in 2 parameters, the `skill` float of the player who hit the ball last, as well as the hit quality. If `over_net()` returns True, `write` the value of the hit quality to Player 2. If it returns False, the point is over, and the receiving player wins the point.

### Player 2 turn
Player 2's turn will look very similar to Player 1's turn. They will call `player_hit` with `player2_skill` and the quality of the previous shot (which is read from a pipe connected to The Net). You'll then write the result of Player 2's hit back to The Net, and the game continues!

## Program Design
There are a number of things to consider when designing this program:
* You will need to figure out where to put your print statements. So long as you're careful, you should have some flexibility here.
* Although the point can be "lost" when a player misses the ball, the point can only be ended by the parent process (The Net). Keep than in mind.
* Make sure that all file descriptors are closed once a point ends. Similarly, ensure that both Player processes are reaped upon completion.