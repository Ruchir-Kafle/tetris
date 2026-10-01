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
#include <math.h>
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

// Blocks state machine
typedef enum {
    SQUARE_PIECE = 1,
    I_PIECE = 2,
    L_PIECE = 3,
    MIRROR_L_PIECE = 4,
    T_PIECE = 5
} blocks_t;



// Update function
// Runs every frame
// For updating everything that must be updated per frame
void update() {

}

// Start point
int main() {

    // Initializing the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris Clone");  
    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose()) {
   

    
        //
        // Drawing
        //

        BeginDrawing();

        ClearBackground(PURPLE);
        
        // Creating grid
        // Grid columns
        for (int i = 0; i < COLUMNS + 1; i++) {
            DrawLine(BLANK_X / 2 + i * BLOCK_SIZE, BLANK_Y / 2, BLANK_X / 2 + i * BLOCK_SIZE, BLANK_Y / 2 + TOTAL_GRID_HEIGHT, BLACK);
        }

        // Grid rows
        for (int i = 0; i < ROWS + 1; i++) {
            DrawLine(BLANK_X / 2, BLANK_Y / 2 + i * BLOCK_SIZE, BLANK_X / 2 + TOTAL_GRID_WIDTH, BLANK_Y / 2 + i * BLOCK_SIZE, BLACK);
        }

        EndDrawing();

    }

    //
    // Termination
    //

    // Closing the window
    CloseWindow();
    
    return 1;
}
