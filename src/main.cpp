#include "Grid.hpp"
#include "Game.hpp"
#include<iostream>
#include "../include/constant.hpp"



int main(){
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);
    


    while(!WindowShouldClose()){SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(WINDOW_TITLE, (GetScreenWidth() - MeasureText(WINDOW_TITLE, 20))/2, 10, 20, DARKGRAY);
        DrawGridFunction(GRID_ROWS, GRID_COLS, CELL_SIZE);
        EndDrawing();
        PollInputEvents(); 
        SwapScreenBuffer();
    }
    


    CloseWindow();

}