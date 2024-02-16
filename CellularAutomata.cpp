#include "CellularAutomata.h"
#include "raylib.h"

#include <iostream>

CellularAutomata::CellularAutomata()
{
    Init();
}

CellularAutomata::~CellularAutomata()
{
    if (cell == nullptr)
    {
        return;
    }

    for (int i = 0; i < MAP_WIDTH; ++i)
    {
        if (cell[i] != nullptr) delete[] cell[i];
    }

    delete[] cell;
}

void CellularAutomata::DoSimulationStep(bool avoidBigSpaces)
{
    int deathLimit {5};
    int overpopLimit {4};
    bool **updatedCell = new bool*[MAP_WIDTH];

	for(int x = 0; x < MAP_WIDTH; ++x)
    {
        updatedCell[x] = new bool[MAP_HEIGHT] { false };
		for(int y = 0; y < MAP_HEIGHT; ++y)
        {
			int nbs = CountAliveNeighbours(x, y);
			if(cell[x][y])
            {
				if(nbs >= overpopLimit || (nbs == 0 && avoidBigSpaces))
                {
 					updatedCell[x][y] = true;
  				}
  			}
  			else
            {
  				if(nbs >= deathLimit)
                {
					updatedCell[x][y] = true;
				}
			}
		}
	}

    std::swap(cell, updatedCell);

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        delete[] updatedCell[x];
    }
    delete[] updatedCell;
}

void CellularAutomata::DrawCellMap()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (!cell[x][y])
            {
                DrawRectangle(x * CELL_SIZE.x, y * CELL_SIZE.y, CELL_SIZE.x, CELL_SIZE.y, CELL_COLOR);
            }
        }
    }
}

void CellularAutomata::DebugPrintCellArray(std::string name)
{
    std::cout << "########## " << name << " ##########" << std::endl;
    for (int i = 0; i < MAP_WIDTH; ++i)
    {
        for (int j = 0; j < MAP_HEIGHT; ++j)
        {
            std::cout << (cell[i][j] ? "X" : " ") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void CellularAutomata::Init()
{
    cell = new bool*[MAP_WIDTH];
    for (int i = 0; i < MAP_WIDTH; ++i)
    {
        cell[i] = new bool[MAP_HEIGHT];
        for (int j = 0; j < MAP_HEIGHT; ++j)
        {
            int randomValue = GetRandomValue(1, 100);
            bool isWall = randomValue < INITIAL_WALL_PROBABILITY;
            cell[i][j] = isWall;
        }
    }
}

const int CellularAutomata::CountAliveNeighbours(const int x, const int y)
{
    int count = 0;
	for(int i = -1; i < 2; ++i)
    {
		for(int j = -1; j < 2; ++j)
        {
			int neighbourX = x + i;
			int neighbourY = y + j;
			if(i == 0 && j == 0)
            {
                continue;
			}
			else if(neighbourX < 0 || neighbourY < 0 || neighbourX >= MAP_WIDTH || neighbourY >= MAP_HEIGHT)
            {
				++count;
			}
			else if(cell[neighbourX][neighbourY])
            {
				++count;
			}
		}
	}

    return count;
}