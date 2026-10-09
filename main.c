// This project is implementing the game Tetris in C,
// In order to do this, the graphics library/game engine Raylib is being used.
// All code was written by Ruchir Kafle.
// The only external resource was the Raylib documentation and examples at:
// https://www.raylib.com/examples.html

// Tetris row is 10 columns by 20 rows
// Each block will be 30px
// Grid will be 300px by 600px

// State machines:
//      Block positioning state (falling, moving side to side, etc)
//      Selected block (which piece does the user currently have)
//      Cell state (occupied or not)

#include "raylib.h" // Graphics library
#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

// Screen size vars
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

// Grid/block measurements in blocks/pixels
// The grid will be centered within the window, as it doesn't take up the entire space
// Grid is ROWS blocks wide, COLUMNS tall, so it will be
// BLOCK_SIZE * COLUMNS by BLOCK_SIZE * ROWS
// Blank space will be SCREEN_WIDTH and SCREEN_HEIGHT minus these respectively
#define ROWS 20
#define COLUMNS 10
#define BLOCK_SIZE 40

#define TOTAL_GRID_WIDTH (COLUMNS * BLOCK_SIZE)
#define TOTAL_GRID_HEIGHT (ROWS * BLOCK_SIZE)
#define BLANK_X (SCREEN_WIDTH - TOTAL_GRID_WIDTH)
#define BLANK_Y (SCREEN_HEIGHT - TOTAL_GRID_HEIGHT)

#define PLACING_TIME 5
#define DEFAULT_SPEED 1 // An action every DEFAULT_SPEED seconds
#define DEFAULT_HORIZONTAL_SPEED 0.1 // An horizontal action every DEFAULT_HORIZONTAL_SPEED seconds

// Blocks state machine
typedef enum {
    EMPTY,
    SQUARE_PIECE,
    I_PIECE,
    L_PIECE,
    J_PIECE,
    T_PIECE
} blocks_t;

typedef enum {
    FALLING = 0,
    PLACED = 1
} block_status_t;

// Game state
typedef enum {
    BEGIN,
    GAME_FALLING,
    INSTANT_FALL,
    SPRINTING,
    MOVING_HORIZONTAL,
    WALL_COLLIDING,
    GAME_PLACING,
    SPAWNING,
    GAME_END,
    PAUSED,
} game_state_t;

// Grid tile
typedef struct {
    blocks_t contents;
    block_status_t status;
} grid_tile_t;


game_state_t game_state = BEGIN;
game_state_t next_game_state;
float counter = 0; // time counter
float horizontal_counter = 0;
float speed = DEFAULT_SPEED; // Lower speed is quicker
int horizontal_direction = 0; // -1 left, 1 right, 0 no lateral movement


// Update function
// Runs every frame
// For updating everything that must be updated per frame
void update() {

}

void commandLineArgs(int argc, char* argv[]) {
    if (argc <= 1) return;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Tetris is a game in which the player is presented with rows and columns in which blocks fall down.\n");
            printf("\nInputs:\n");
            printf("Left Arrow Key: Lateral left movement\n");
            printf("Right Arrow Key: Lateral right movement\n");
            printf("Down Arrow Key: Quick downwards vertical movement\n");
            printf("Spacebar Key: Instant block placement.\n");
            exit(0);
        }
    }
}

// Start point
int main(int argc, char *argv[]) {

    commandLineArgs(argc, argv);

    // Instantiate an EMPTY grid
    grid_tile_t *tetris_grid[ROWS][COLUMNS];

    for (int i = 0; i < ROWS; i++) {

        for (int j = 0; j < COLUMNS; j++) {
            grid_tile_t *new_tile = (grid_tile_t *)malloc(sizeof(grid_tile_t));

            if (new_tile == NULL) {
                printf("Could not allocate tile memory");
                exit(1);
            }

            new_tile->contents = EMPTY;
            new_tile->status = PLACED;

            tetris_grid[i][j] = new_tile;
        } 
    }


    // Initializing the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone");  
    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        counter += dt;
        horizontal_counter += dt;

        printf("%d", game_state);
        
        // User input
        if (game_state <= GAME_PLACING) {
            if (IsKeyDown(KEY_LEFT)) {horizontal_direction = -1; next_game_state = MOVING_HORIZONTAL;}
            else if (IsKeyDown(KEY_RIGHT)) {horizontal_direction = 1; next_game_state = MOVING_HORIZONTAL;}
            else if (IsKeyDown(KEY_DOWN)) {next_game_state = SPRINTING;}
            else if (IsKeyPressed(KEY_ESCAPE)) {next_game_state = PAUSED;}
            else if (IsKeyPressed(KEY_SPACE)) {next_game_state = INSTANT_FALL;}
        } else if (game_state == GAME_END) {
            if (IsKeyPressed(KEY_ENTER)) {next_game_state = BEGIN;}
        } else if (game_state == PAUSED) {
            if (IsKeyPressed(KEY_ESCAPE)) {next_game_state = GAME_FALLING;}
        }


        if (game_state == BEGIN) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLUMNS; j++) {
                    tetris_grid[i][j]->contents = EMPTY;
                    tetris_grid[i][j]->status = PLACED;
                }
            }

            next_game_state = SPAWNING;
        }
        // Logic for active game
        else if (game_state != GAME_END && game_state != PAUSED && game_state != SPAWNING) {
            for (int i = ROWS-1; i >= 0; i--) {
                for (int j = horizontal_direction == 1 ? COLUMNS - 1 : 0;
                    horizontal_direction == 1 ? j >= 0 : j <= COLUMNS - 1;
                    j += horizontal_direction == 0 ? 1 : -horizontal_direction) {

                    // Normal tetris movement

                    if (horizontal_counter >= DEFAULT_HORIZONTAL_SPEED) {
                        if (tetris_grid[i][j]->contents != EMPTY && tetris_grid[i][j]->status == FALLING) {
                            int deltaX = 0;
                            if (game_state == WALL_COLLIDING) {
                                next_game_state = -1;
                            } else {
                                if (j < COLUMNS-1 && horizontal_direction == 1) {
                                    deltaX = horizontal_direction;
                                } else if (j > 0 && horizontal_direction == -1) {
                                    deltaX = horizontal_direction;
                                } else if (j != 0) {
                                    next_game_state = WALL_COLLIDING;
                                }

                                if (j + 1 == COLUMNS - 1 || j - 1 == 0)
                                    next_game_state = WALL_COLLIDING;
                            }
                            printf("print: %d\n", deltaX);
                            if (deltaX != 0) {

                                tetris_grid[i][j+deltaX]->contents = tetris_grid[i][j]->contents;
                                tetris_grid[i][j+deltaX]->status = tetris_grid[i][j]->status;

                                tetris_grid[i][j]->contents = EMPTY;
                                tetris_grid[i][j]->status = PLACED;
                            }
                        }
                    }

                    if (game_state != GAME_PLACING && counter >= speed) {
                        if (tetris_grid[i][j]->contents != EMPTY && tetris_grid[i][j]->status == FALLING) {
                            int deltaY = 0;
                            
                            if (i < ROWS - 1 && tetris_grid[i+1][j]->contents == EMPTY) { 
                                deltaY = 1;
                            } else {
                                next_game_state = GAME_PLACING;
                            } 

                            if (deltaY != 0) {
                                tetris_grid[i+deltaY][j]->contents = tetris_grid[i][j]->contents;
                                tetris_grid[i+deltaY][j]->status = tetris_grid[i][j]->status;

                                tetris_grid[i][j]->contents = EMPTY;
                                tetris_grid[i][j]->status = PLACED;
                            }
                        }
                    } else if (game_state == GAME_PLACING && counter >= 1) { 
                        if (tetris_grid[i][j]->status == FALLING) {
                            tetris_grid[i][j]->status = PLACED;
                            next_game_state = SPAWNING;
                        }

                    }
                }
            }
            
            if (game_state == INSTANT_FALL) {
                speed = 0;
                next_game_state = INSTANT_FALL;
            } else if (game_state == SPRINTING) {
                speed = DEFAULT_SPEED / 2.0;
            } else {
                speed = DEFAULT_SPEED;
            }

        }  else if (game_state == SPAWNING) {

            int randomBlock = GetRandomValue(1, 5);
            int middleColumn = (int) COLUMNS / 2;

            switch (randomBlock) {
                case (SQUARE_PIECE):
                    for (int i = 0; i <= 1; i++) {
                        for (int j = 0; j <= 1; j++) {         
                            tetris_grid[i][middleColumn-j]->contents = SQUARE_PIECE;
                            tetris_grid[i][middleColumn-j]->status = FALLING;
                        } 
                    }
                    break;

                case (I_PIECE):
                    for (int j = -1; j <= 2; j++) {         
                        tetris_grid[0][middleColumn+j]->contents = I_PIECE;
                        tetris_grid[0][middleColumn+j]->status = FALLING;
                    } 
                    break;


                case (L_PIECE):
                    for (int i = 0; i <= 2; i++) {         
                        tetris_grid[i][middleColumn-1]->contents = L_PIECE;
                        tetris_grid[i][middleColumn-1]->status = FALLING;
                    } 
                    tetris_grid[2][middleColumn]->contents = L_PIECE;
                    tetris_grid[2][middleColumn]->status = FALLING;
                    break;

                case (J_PIECE):
                    for (int i = 0; i <= 2; i++) {         
                        tetris_grid[i][middleColumn]->contents = J_PIECE;
                        tetris_grid[i][middleColumn]->status = FALLING;
                    } 
                    tetris_grid[2][middleColumn-1]->contents = J_PIECE;
                    tetris_grid[2][middleColumn-1]->status = FALLING;
                    break;

                case (T_PIECE):
                    for (int j = -1; j <= 1; j++) {         
                        tetris_grid[0][middleColumn+j]->contents = T_PIECE;
                        tetris_grid[0][middleColumn+j]->status = FALLING;
                    }
                    
                    tetris_grid[1][middleColumn]->contents = T_PIECE;
                    tetris_grid[1][middleColumn]->status = FALLING;
                    break;

                default:
                    break;
                
            }
             
            next_game_state = GAME_FALLING;

        }
   
        // Setting the new game state after program execution is finished
        if (next_game_state != -1) {
            game_state = next_game_state;
        }
        next_game_state = -1;
        horizontal_direction = 0;

        if (counter >= speed) {
            counter -= speed;
        }
        if (horizontal_counter >= DEFAULT_HORIZONTAL_SPEED) {
            horizontal_counter -= DEFAULT_HORIZONTAL_SPEED;
        }

        //
        // Drawing
        //

        BeginDrawing();

        ClearBackground(PURPLE);
        
        // Creating grid
        // Grid columns
        for (int i = 0; i < COLUMNS + 1; i++) {
            DrawLine(BLANK_X / 2 + i * BLOCK_SIZE, 
                     BLANK_Y / 2, 
                     BLANK_X / 2 + i * BLOCK_SIZE, 
                     BLANK_Y / 2 + TOTAL_GRID_HEIGHT, 
                     BLACK);
        }

        // Grid rows
        for (int i = 0; i < ROWS + 1; i++) {
            DrawLine(BLANK_X / 2,
                     BLANK_Y / 2 + i * BLOCK_SIZE, 
                     BLANK_X / 2 + TOTAL_GRID_WIDTH, 
                     BLANK_Y / 2 + i * BLOCK_SIZE, 
                     BLACK);
        }


        for (int i = 0; i < ROWS; i++) {

            for (int j = 0; j < COLUMNS; j++) {
    
                Color color;

                switch (tetris_grid[i][j]->contents) {
                    case (EMPTY):
                        color = WHITE;
                        break;
                    
                    case (SQUARE_PIECE):
                        color = YELLOW;
                        break;

                    case (I_PIECE):
                        color = BLUE;
                        break;

                    case (L_PIECE):
                        color = PINK;
                        break;

                    case (J_PIECE):
                        color = ORANGE;
                        break;

                    case (T_PIECE):
                        color = GREEN;
                        break;

                    default:
                        color = RED;
                }

                DrawRectangle(BLANK_X / 2 + j * BLOCK_SIZE, 
                              BLANK_Y / 2 + i * BLOCK_SIZE, 
                              BLOCK_SIZE - 1, 
                              BLOCK_SIZE - 1, color);
            }
        }

        EndDrawing();

    }

    //
    // Termination
    //

    // Closing the window
    CloseWindow();
    
    return 0;
}
