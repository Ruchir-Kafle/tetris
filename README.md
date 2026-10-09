# Ruchir Kafle's Tetris Clone

Of course, this game is not an original creation. The idea of Tetris has a long history before I was even born, the only thing I will claim as my own is the code and the documentation, which I wrote myself. This is a mere copy of the popular game with some features taken out in order to make the project more feasible within the limited time frame given.

Tetris is a game in which the player is presented with rows and columns in which blocks fall down. These blocks begin from the top of the grid, spawning and moving down one at a time. Once a randomly generated piece from a list of options reaches the bottom of the grid, it will begin to place, permanently setting it in place. As the player stacks blocks higher and higher, the grid will begin to fill, blocks stacking upon one another and placing sooner due to other blocks being valid placement spots beyond the floor of the grid. When a row fills, however, it will be cleared in full, the placed blocks above it will drop by a row, and the user will be awarded points. The goal of the game is to earn as many points as possible, which of course comes with surviving with a clean grid for as long as possible.

# Inputs/How to Play

Left Arrow Key: Lateral left movement
Right Arrow Key: Lateral right movement
Down Arrow Key: Quick downwards vertical movement
Spacebar Key: Instant block placement.
P Key: Pause.

# Building Procedure

1. Begin by running the Makefile using the following command in a terminal at the root directory of this project:

`make`

This, if the user has GCC installed on their device, should compile the C code directly into a final executable of the game.


2. Running the project is simple, use the command:

`./main`

For information regarding keybinds and command line arguments, use the `-h` or `--help` flags, like so:

`./main -h` or `./main --help`
