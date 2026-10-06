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
#include <stdlib.h>
#include <time.h>

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
#define DEFAULT_SPEED 2

// Blocks state machine
typedef enum {
    EMPTY = 0,
    SQUARE_PIECE = 1,
    I_PIECE = 2,
    L_PIECE = 3,
    MIRROR_L_PIECE = 4,
    T_PIECE = 5
} blocks_t;

typedef enum {
    FALLING = 0,
    PLACED = 1
} block_status_t;

// Game state
typedef enum {
    BEGIN,
    GAME_FALLING,
    SPRINTING,
    MOVING_HORIZONTAL,
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


game_state_t game_state;
game_state_t next_game_state;
float counter = 0;
int speed = DEFAULT_SPEED;
int horizontal_direction = 0;


// Update function
// Runs every frame
// For updating everything that must be updated per frame
void update() {

}

// Start point
int main() {

    // Instantiate an EMPTY grid
    grid_tile_t *tetris_grid[ROWS][COLUMNS];

    for (int i = 0; i < ROWS; i++) {

        for (int j = 0; j < COLUMNS; j++) {
            grid_tile_t *new_tile = (grid_tile_t *)malloc(sizeof(grid_tile_t));

            if (new_tile == NULL) {
                exit(1);
            }

            new_tile -> contents = EMPTY;

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

        // User input
        if (game_state <= GAME_PLACING) {
            if (IsKeyDown(KEY_LEFT)) {horizontal_direction = -1; next_game_state = MOVING_HORIZONTAL;}
            else if (IsKeyDown(KEY_RIGHT)) {horizontal_direction = 1; next_game_state = MOVING_HORIZONTAL;}
            else if (IsKeyDown(KEY_DOWN)) {next_game_state = SPRINTING;}
            else if (IsKeyPressed(KEY_ESCAPE)) {next_game_state = PAUSED;}
            else if (IsKeyPressed(KEY_SPACE)) {next_game_state = SPRINTING;} // <- need to make this instant
        } else if (game_state == GAME_END) {
            if (IsKeyPressed(KEY_ENTER)) {next_game_state = BEGIN;}
        } else if (game_state == PAUSED) {
            if (IsKeyPressed(KEY_ESCAPE)) {next_game_state = GAME_FALLING;}
        }


        // Logic for active game
        if (game_state != GAME_END && game_state != PAUSED) {
            for (int i = ROWS; i > 0; i--) {
                for (int j = 0; j < COLUMNS; j++) {
                    
                    if (game_state != GAME_PLACING && counter >= speed) {
                        if (tetris_grid[i][j]->contents != EMPTY && tetris_grid[i][j]->status == FALLING) {
                            if (tetris_grid[i+1][j]->contents == EMPTY) {
                                
                                tetris_grid[i+1][j]->contents = tetris_grid[i][j]->contents;
                                tetris_grid[i][j]->contents = EMPTY;
                            } else {
                                next_game_state = GAME_PLACING;
                            }
                        }
                    } else if (game_state == GAME_PLACING && counter >= speed * 2) { 
                        if (tetris_grid[i][j]->status == FALLING) {
                            tetris_grid[i][j]->status = PLACED;
                            next_game_state = SPAWNING;
                        }

                    }


            }

            if (game_state == SPRINTING) {
                speed = DEFAULT_SPEED / 2;
            } else {
                speed = DEFAULT_SPEED;
            }

        }
   
        // Setting the new game state after program execution is finished
        game_state = next_game_state;

        if (counter >= speed) {
            counter -= speed;
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

                    case (MIRROR_L_PIECE):
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
