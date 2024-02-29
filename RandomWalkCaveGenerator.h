#pragma once

#include <vector>
#include "raylib.h"

#include "Cell.h"

struct Walker
{
    int PosX {};
    int PosY {};
    int StepsLeft {};
    std::vector<int> RecordedXPos {};
    std::vector<int> RecordedYPos {};
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
    
    void PrepareMapGeneration();
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
    void OptimizeFloorCells();
    const bool HasFloorNeighbours(const int x, const int y);
    const std::map<ECellVertice, float> GetNeighboursPosY(const int x, const int y);

    const int MAP_WIDTH {128};
    const int MAP_HEIGHT {72};
    const Vector2 CELL_SIZE {10.0f, 10.0f};
    const Color CELL_COLOR {WHITE}; // for non-wall cell
    const int WALKER_NB = 1; // 10
    const int STEPS_PER_WALKER = 2; // 600
    const int DELAY_BETWEEN_SPAWN = 0; // in steps

    int optimizedWidth {-1};
    int optimizedHeight {-1};
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