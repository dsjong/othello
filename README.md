# Othello

GitHub: https://github.com/dsjong/othello/

## Collaborators

* Dion Ong: Implemented the game and MTDF on top of minimax

* Andrew Wang: Implemented the minimax engine

## Description

Othello is a two player strategy game played on an 8×8 board. Read the rules [here](https://www.worldothello.org/about/about-othello/othello-rules/official-rules/english).

## Testing

To run the test script:

1. Run `git clone git@github.com:dsjong/othello.git`.

2. Run `make` to create the `bin/runner` executable. Optionally, run `make CFLAGS=-DDEBUG` after doing `make clean` to see debugging outputs.

3. Run `./bin/runner num_games engine1 engine2 seed`. The runner outputs the individual scores of `2 * num_games` games, as well as the cumulative points gained by each agent. 1 point is awarded for a win, 0.5 for a draw, and 0 for a loss.

   * `num_games` -- the number of **pairs** of games to be run. For fairness, both engines start one game in each set first and another game second.

   * `engine1` -- the first engine to play. The allowed engines are listed in the following table:

     | Engine    | Description                                               |
     | --------- | --------------------------------------------------------- |
     | `ab`      | Alpha-beta agent with transposition table                 |
     | `minimax` | [MTDF](http://people.csail.mit.edu/plaat/mtdf.html) agent |
     | `mtdf`    | Minimax agent                                             |
     | `random`  | Random agent                                              |

   * `engine2`-- the second engine to play (same choices as the first engine).

   * `seed` -- positive integer seed used to randomize the starting board. If seed is `0`, no randomization is used and the game starts from the standard starting position.

Examples: `./bin/runner 20 mtdf minimax 1`

## Results

We found the MTDF agent to be the strongest, followed by alpha beta with transposition table then minimax.

Playing 50 sets of games between the three agents (using a seed of `1`) gives the following results (see details in the `log` folder):

| Engine  | MTDF     | AB    | Minimax  | Total      |
| ------- | -------- | ----- | -------- | ---------- |
| MTDF    |          | 59-41 | 98.5-1.5 | 157.5-42.5 |
| AB      | 41-59    |       | 100-0    | 141-59     |
| Minimax | 1.5-98.5 | 0-100 |          | 1.5-198.5  |

All three agents beat the random agent almost 100% of the time starting from the standard initial position. To verify this, run the test script with seed `0` (no randomization is needed because the benchmark agent is already random).
