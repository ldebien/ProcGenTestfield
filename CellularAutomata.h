#pragma once // CELLULAR_AUTOMATA_H

#include "raylib.h"
#include <string>

class CellularAutomata
{
public:
    CellularAutomata();
    ~CellularAutomata();

    void DoSimulationStep(bool avoidBigSpaces);
    void DrawCellMap();
    void DebugPrintCellArray(std::string name);
private:
    const int MAP_WIDTH {128};
    const int MAP_HEIGHT {72};
    const int NB_STEPS {4};
    const int INITIAL_WALL_PROBABILITY {45};

    const Vector2 CELL_SIZE {10.0f, 10.0f};
    const Color CELL_COLOR {WHITE}; // for non-wall cell

    bool **cell;

    void Init();
    const int CountAliveNeighbours(const int x, const int y);
}; // CELLULAR_AUTOMATA_H