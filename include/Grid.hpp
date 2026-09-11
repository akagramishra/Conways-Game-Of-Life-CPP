#pragma once

#ifndef GRID_H
#define GRID_H

#include<raylib.h>


void DrawGridFunction(int rows, int cols, int cellSize);

std::vector<Vector2> Vector2ToGridCoordinates(Vector2 position, int cellSize, int rows, int cols);



#endif //Definition Close here

