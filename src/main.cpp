#include "Grid.h"
#include "Game.h"
#include<iostream>
#include "../include/constant.hpp"



int main(){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's Game of Life");
    SetTargetFPS(60);
    


    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawText("Conway's Game of Life", 10, 10, 20, DARKGRAY);
        DrawGridFunction(GRID_ROWS, GRID_COLS, CELL_SIZE);
        EndDrawing();
        PollInputEvents(); 
        SwapScreenBuffer();
    }
    


    CloseWindow();

}