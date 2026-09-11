#include "Grid.hpp"

void DrawGridFunction(int rows, int cols, int cellSize)
{
    for (int i = 2; i <= rows; ++i) {
        DrawLine(0, i * cellSize, cols * cellSize, i * cellSize, LIGHTGRAY);
    }

    for (int j = 1; j <= cols; ++j) {
        DrawLine(j * cellSize, 2*cellSize, j * cellSize, rows * cellSize, LIGHTGRAY);
    }
}

std::vector<Vector2> Vector2ToGridCoordinates(Vector2 position, int cellSize, int rows, int cols)
{
    std::vector<Vector2> gridCoordinates;

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            Vector2 cellPosition = { j * cellSize, i * cellSize };
            if (position.x >= cellPosition.x && position.x < cellPosition.x + cellSize &&
                position.y >= cellPosition.y && position.y < cellPosition.y + cellSize) {
                gridCoordinates.push_back(cellPosition);
            }
        }
    }

    return gridCoordinates;
}
