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
2. The float `previous_shot_difficulty`, which measures how difficult the previous shot was (0.0 - easiest to 1.0 - impossible). For example, if Player 2 hits a fantastic shot, player 1 will have a difficult time returning it successfully, regardless of `player1_skill`. For the first turn, you may assume that `previous_shot_difficulty` is 0.0.

Use these as the arguments to the `player_hit()` function, which returns a float representing the quality of Player 1's hit. For example:
```
float player1_hit_quality = player_hit (player1_skill, previous_shot_difficulty);
```

Next is where things get fun. If the result is True, you will write the char value "1" to the parent. Simiarly, if the result is False (miss), you will write a char value "0" to the parent.

### The Net turn
To make things even more fun, The Net can reject balls hit from either player. The Net will reject a ball that is *successfully* hit from either player if the function `over_net()` returns false. This function takes in a single parameter, the `skill` float of the player who hit the ball last. If `over_net()` returns True, 

TODO
Figure out how we encode the answer? do you just write a 1 or a 0? That's all you need
Leave it up to them to decide when to print...
