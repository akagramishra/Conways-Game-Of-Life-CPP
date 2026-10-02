#include <vector>
#include <utility>
#include<string>
#include "Game.hpp"

void Simulation::Draw()
{
    DrawText("Live Cells:", 10, 10, 20, Color{0, 255, 0, 255});
    DrawText(std::to_string(CountLiveCells()).c_str(), MeasureText("Live Cells: 0",20)+1, 10, 20, Color{0, 0, 255, 235});
    DrawText((IsRunning() ? "Running" : "Paused"), GetScreenWidth() - MeasureText((IsRunning() ? "Running" : "Paused"), 20) - 10, 10, 20, (IsRunning() ? Color{0, 255, 0, 255} : Color{255, 0, 0, 255}));
    grid.Draw();
}

void Simulation::SetCellValue(int row, int column, int value)
{
    grid.SetValue(row, column, value);
}

int Simulation::CountLiveNeighbors(int row, int column)
{
    int liveNeighbors = 0;
    std::vector<std::pair<int, int>> neighborOffsets = 
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1} 
    };

    for(const auto& offset : neighborOffsets)
    {
        int neighborRow = (row + offset.first + grid.GetRows()) % grid.GetRows();
        int neighborColumn = (column + offset.second + grid.GetColumns()) % grid.GetColumns();
        liveNeighbors += grid.GetValue(neighborRow, neighborColumn);
    }
    return liveNeighbors;
}

void Simulation::Update()
{
    if(IsRunning())
    {
        for(int row = 0; row < grid.GetRows(); row++)
        {
            for(int column = 0; column < grid.GetColumns(); column++)
            {
                int liveNeighbors = CountLiveNeighbors(row, column);
                int cellValue = grid.GetValue(row, column);
                
                if(cellValue == 1)
                {
                    if(liveNeighbors > 3 || liveNeighbors < 2)
                    {
                        tempGrid.SetValue(row, column, 0);
                    }
                    else
                    {
                        tempGrid.SetValue(row, column, 1);
                    }
                }
                else
                {
                    if(liveNeighbors == 3)
                    {
                        tempGrid.SetValue(row, column, 1);
                    }
                    else
                    {
                        tempGrid.SetValue(row, column, 0);
                    }
                }
            }
        }
        grid = tempGrid;
    }
}

void Simulation::ClearGrid()
{
    if(!IsRunning())
    {
        grid.Clear();
    }
}

void Simulation::CreateRandomState()
{
    if(!IsRunning())
    {
        grid.FillRandom();
    }
}

void Simulation::ToggleCell(int row, int column)
{
    if(!IsRunning())
    {
        grid.ToggleCell(row, column);
    }
}

int Simulation::CountLiveCells()
{
    int liveCellCount = 0;
    for(int row = 0; row < grid.GetRows(); row++)
    {
        for(int column = 0; column < grid.GetColumns(); column++)
        {
            liveCellCount += grid.GetValue(row, column);
        }
    }
    return liveCellCount;
}
