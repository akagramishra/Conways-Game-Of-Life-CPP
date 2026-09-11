#include "Grid.h"
#include<vector>

void DrawGridFunction(int rows, int cols, int cellSize)
{
    for (int i = 0; i <= rows; ++i) {
        DrawLine(0, i * cellSize, cols * cellSize, i * cellSize, LIGHTGRAY);
    }

    for (int j = 0; j <= cols; ++j) {
        DrawLine(j * cellSize, 0, j * cellSize, rows * cellSize, LIGHTGRAY);
    }
}

void Vector2ToGridCoordinates(Vector2 position, int cellSize, int &row, int &col)
{
    
}
