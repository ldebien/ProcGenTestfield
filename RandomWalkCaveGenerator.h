#pragma once

#include <vector>
#include "raylib.h"

#include "Cell.h"

struct Walker
{
    int PosX {};
    int PosY {};
    int StepsLeft {};
};

class RandomWalkCaveGenerator
{
public:
    RandomWalkCaveGenerator();
    ~RandomWalkCaveGenerator();

    const int GetOptimizedWidth()   { return optimizedWidth; }
    const int GetOptimizedHeight()  { return optimizedHeight; }
    const int GetMinX()             { return minX; }
    const int GetMaxX()             { return maxX; }
    const int GetMinY()             { return minY; }
    const int GetMaxY()             { return maxY; }
    const bool IsFinished()         { return finished; };
    
    void GenerateMap();
    void GenerateOneStep();
    void FullyGenerateMap();

    void DrawCellMap();
    void DrawWalkers();
    void DrawOptimizedMapBoundaries();
    void DrawCellMap3D();
    void DrawWalkers3D();

private:
    void Init();
    void GenerateFloor();
    void SpawnWalker();
    void PostStepCheck();
    void OptimizeFloor();

    const int MAP_WIDTH {128};
    const int MAP_HEIGHT {72};
    const Vector2 CELL_SIZE {10.0f, 10.0f};
    const Color CELL_COLOR {WHITE}; // for non-wall cell
    const int WALKER_NB = 10;
    const int STEPS_PER_WALKER = 600;
    const int DELAY_BETWEEN_SPAWN = 233; // in steps

    int optimizedWidth {};
    int optimizedHeight {};
    int minX, maxX {};
    int minY, maxY {};

    Cell **floorCells;

    //bool **cell;
    Vector3 **cellPos;
    Image floorPerlinNoiseImage {};

    std::vector<Walker*> m_walkers {};

    // Game loop generation related var
    bool finished {false};
    int curStep {0};
    int timeout {0};
};