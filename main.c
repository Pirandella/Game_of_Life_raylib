#include "raylib.h"

#define WIN_WIDTH   800
#define WIN_HEIGHT  470

#define BAR_SIZE    20

#define GAME_WIN_WIDTH  WIN_WIDTH
#define GAME_WIN_HEIGHT (WIN_HEIGHT - BAR_SIZE)

#define CELL_SIZE       10
#define MIN_GAME_SPEED  1
#define MAX_GAME_SPEED  60

int board0[GAME_WIN_WIDTH][GAME_WIN_HEIGHT];
int board1[GAME_WIN_WIDTH][GAME_WIN_HEIGHT];

void clearBoards(void);
void genRandCells(void);

int countCells(int x, int y, int n);
void rulesCheck(int n);

int main(void){

    Vector2 mousePos;
    Vector2 barTextPos = {3.0, GAME_WIN_HEIGHT + 6};

    int gameState = 0;
    int boardNumber = 0;

    int frameCounter = 0;
    int gameSpeed = 10;
    int genNumber = 0;

    Font barFont;

    clearBoards();

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "GameOfLife");

    barFont = LoadFontEx("./fonts/UniqueenBold.ttf", 13, 0, 250);

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        frameCounter++;
        if(frameCounter >= (60 / gameSpeed)){
            frameCounter = 0;

            if(gameState){
                rulesCheck(boardNumber);
                boardNumber ^= 1;
                genNumber++;
            }
        }

        if(!gameState){
            mousePos = GetMousePosition();
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(!boardNumber)
                    board0[(int)(mousePos.x / CELL_SIZE)][(int)(mousePos.y / CELL_SIZE)] = 1;
                else
                    board1[(int)(mousePos.x / CELL_SIZE)][(int)(mousePos.y / CELL_SIZE)] = 1;
            }else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
                if(!boardNumber)
                    board0[(int)(mousePos.x / CELL_SIZE)][(int)(mousePos.y / CELL_SIZE)] = 0;
                else
                    board1[(int)(mousePos.x / CELL_SIZE)][(int)(mousePos.y / CELL_SIZE)] = 0;
            }
        }

        if(IsKeyReleased(KEY_SPACE)){
            gameState ^= 1;
        }else if(IsKeyReleased(KEY_C)){
            gameState = 0;
            genNumber = 0;
            clearBoards();
        }else if(IsKeyReleased(KEY_R)){
            gameState = 0;
            genNumber = 0;
            genRandCells();
        }else if(IsKeyReleased(KEY_LEFT_BRACKET)){
            gameSpeed--;
            if(gameSpeed < MIN_GAME_SPEED) gameSpeed = MIN_GAME_SPEED;
        }else if(IsKeyReleased(KEY_RIGHT_BRACKET)){
            gameSpeed++;
            if(gameSpeed > MAX_GAME_SPEED) gameSpeed = MAX_GAME_SPEED;
        }

/*-----------------------------------------------------------------------------------------------------*/
        BeginDrawing();
        ClearBackground(BEIGE);

        // Draw grid
        for(int i = 0; i < GAME_WIN_WIDTH; i += CELL_SIZE) DrawLine(i, 0, i, GAME_WIN_HEIGHT, DARKBROWN);
        for(int i = 0; i <= GAME_WIN_HEIGHT; i += CELL_SIZE) DrawLine(0, i, GAME_WIN_WIDTH, i, DARKBROWN);

        // Draw Cells
        for(int i = 0; i < GAME_WIN_WIDTH; i += CELL_SIZE){
            for(int j = 0; j < GAME_WIN_HEIGHT; j += CELL_SIZE){
                if(boardNumber == 0){
                    if(board0[i / 10][j / 10])
                        DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, DARKBROWN);
                }else{
                    if(board1[i / 10][j / 10])
                        DrawRectangle(i, j, CELL_SIZE, CELL_SIZE, DARKBROWN);
                }
            }
        }

        DrawTextEx(barFont, FormatText("State: %4s, Speed: %2d Gen: %4d | LMB - place cell, RMB - remove cell, SPACE - Run/Stop, C - Clear, R - Gen Rand, [/] - Game Speed -/+, ESC - Exit",
                            ((gameState) ? "Run" : "Stop"), gameSpeed, genNumber), barTextPos, 13, 1, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
/*-----------------------------------------------------------------------------------------------------*/
void clearBoards(){
    for(int i = 0; i < GAME_WIN_WIDTH; i++){
        for(int j = 0; j < GAME_WIN_HEIGHT; j++){
            board0[i][j] = 0;
            board1[i][j] = 0;
        }
    }
}

void genRandCells(void){
    clearBoards();
    for(int i = 0; i < GAME_WIN_WIDTH; i++){
        for(int j = 0; j < GAME_WIN_HEIGHT; j++){
            board0[i][j] = GetRandomValue(0, 1);
        }
    }
}

int countCells(int x, int y, int n){
    int sum = 0;
    int line, cols;
    if(!n){
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                cols = (x + i + GAME_WIN_WIDTH) % GAME_WIN_WIDTH;
                line = (y + j + GAME_WIN_HEIGHT) % GAME_WIN_HEIGHT;
                if(board0[cols][line] == 1) sum++;
            }
        }
        if(board0[x][y] == 1) sum--;
    }else{
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                cols = (x + i + GAME_WIN_WIDTH) % GAME_WIN_WIDTH;
                line = (y + j + GAME_WIN_HEIGHT) % GAME_WIN_HEIGHT;
                if(board1[cols][line] == 1) sum++;
            }
        }
        if(board1[x][y] == 1) sum--;
    }

    return sum;
}

void rulesCheck(int n){
    int cell = 0;
    int isAlive = 0;
    for(int i = 0; i < GAME_WIN_WIDTH; i++){
        for(int j = 0; j < GAME_WIN_HEIGHT; j++){
            cell = countCells(i, j, n);
            if(!n){
                if(board0[i][j]) isAlive = 1;
                else isAlive = 0;

                if(!isAlive && (cell == 3)) board1[i][j] = 1;
                else if(isAlive && (cell == 2 || cell == 3)) board1[i][j] = 1;
                else board1[i][j] = 0;
            }else{
                if(board1[i][j]) isAlive = 1;
                else isAlive = 0;

                if(!isAlive && (cell == 3)) board0[i][j] = 1;
                else if(isAlive && (cell == 2 || cell == 3)) board0[i][j] = 1;
                else board0[i][j] = 0;
            }
        }
    }
}
