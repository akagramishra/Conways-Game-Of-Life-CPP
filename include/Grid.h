#pragma once

#ifndef GRID_H
#define GRID_H

#include<raylib.h>


void DrawGridFunction(int rows, int cols, int cellSize);

void Vector2ToGridCoordinates(Vector2 position, int cellSize, int& row, int& col);



#endif //Definition Close here

