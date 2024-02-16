#include "raylib.h"
#include "raymath.h"

#include <iostream>

#include "RandomWalkCaveGenerator.h"

RandomWalkCaveGenerator::RandomWalkCaveGenerator()
{
    Init();
}

RandomWalkCaveGenerator::~RandomWalkCaveGenerator()
{
    if (floorCells != nullptr)
    {
        for (int i = 0; i < MAP_WIDTH; ++i)
        {
            if (floorCells[i] != nullptr) delete[] floorCells[i];
        }

        delete[] floorCells;
    }

    for (Walker * walker : m_walkers)
    {
        delete walker;
    }
    m_walkers.clear();
}

/// @brief Prepare map generation in order to generate in game loop, one step per tick
void RandomWalkCaveGenerator::GenerateMap()
{
    //SetRandomSeed(91620);

    m_walkers = {};
    SpawnWalker();

    finished = false;
    timeout = MAP_WIDTH * MAP_HEIGHT;
    curStep = 0;
}

/// @brief Generate the fully map in one tick
void RandomWalkCaveGenerator::FullyGenerateMap()
{
    //SetRandomSeed(91620);

    m_walkers.clear();
    SpawnWalker();
    finished = false;
    timeout = MAP_WIDTH * MAP_HEIGHT;
    curStep = 0;

    while (timeout > 0 && !finished)
    {
        if (curStep >= timeout)
        {
            break;
        }

        GenerateOneStep();

        curStep++;
        if (curStep >= static_cast<int>(m_walkers.size()) * DELAY_BETWEEN_SPAWN && static_cast<int>(m_walkers.size()) < WALKER_NB)
        {
            SpawnWalker();
        }

        bool done = true;
        for (Walker * walker : m_walkers)
        {
            done &= walker->StepsLeft <= 0;
        }

        finished = done;
    }

    OptimizeFloor();
}

void RandomWalkCaveGenerator::DrawCellMap()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            floorCells[x][y].Draw2D(Vector2Zero(), CELL_SIZE.x, CELL_SIZE.y);

            // if (cell[x][y])
            // {
            //     DrawRectangle(x * CELL_SIZE.x, y * CELL_SIZE.y, CELL_SIZE.x, CELL_SIZE.y, CELL_COLOR);
            // }
        }
    }
}

void RandomWalkCaveGenerator::DrawWalkers()
{
    for (Walker * walker : m_walkers)
    {
        DrawRectangle(walker->PosX * CELL_SIZE.x, walker->PosY * CELL_SIZE.y, CELL_SIZE.x, CELL_SIZE.y, BLUE);
    }
}

void RandomWalkCaveGenerator::DrawOptimizedMapBoundaries()
{
    Color debugColor = GREEN;
    debugColor.a = (unsigned char)64;
    DrawRectangle(minX * 10.0f, minY * 10.0f, (optimizedWidth + 1) * 10.0f, (optimizedHeight + 1) * 10.0f, debugColor);
}

void RandomWalkCaveGenerator::DrawCellMap3D()
{
    Vector3 offset {MAP_WIDTH * 0.5f, 4.0f, MAP_HEIGHT * 0.5f};
    floorPerlinNoiseImage = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 0, 0, 4.0f);
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (!floorCells[x][y].IsFloor()) continue;

            // Color pixelColor = GetImageColor(floorPerlinNoiseImage, x, y);
            // Vector3 centerPos = {x - offset.x + 0.5f, pixelColor.r / 32 - offset.y, y - offset.z + 0.5f};
            // //DrawPlane(centerPos, cellSize, GetImageColor(perlinNoiseImage, x, y));
            // DrawCube(centerPos, 1.0f, 1.0f, 1.0f, GetImageColor(floorPerlinNoiseImage, x, y));
            floorCells[x][y].Draw3D(offset);
        }
    }
}

void RandomWalkCaveGenerator::DrawWalkers3D()
{
    Vector3 offset {MAP_WIDTH * 0.5f, 4.0f, MAP_HEIGHT * 0.5f};
    for (Walker * walker : m_walkers)
    {
        int x = walker->PosX;
        int y = walker->PosY;
        Vector3 walkerPos {x - offset.x + 0.5f , GetImageColor(floorPerlinNoiseImage, x, y).r / 32 - offset.y + 0.75f, y - offset.z + 0.5f};
        DrawCube(walkerPos, 0.5f, 0.5f, 0.5f, GOLD);
    }
}

void RandomWalkCaveGenerator::Init()
{
    int total = 0;
    std::cout << "Number of values to initialize: " << MAP_WIDTH * MAP_HEIGHT << std::endl;
    floorPerlinNoiseImage = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 0, 0, 4.0f);

    floorCells = new Cell*[MAP_WIDTH];
	for(int x = 0; x < MAP_WIDTH; ++x)
    {
        floorCells[x] = new Cell[MAP_HEIGHT] {};
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            Color pixelColor = GetImageColor(floorPerlinNoiseImage, x, y);
            Vector3 pos { x + 0.5f, pixelColor.r / 32, y + 0.5f };
            floorCells[x][y].Init(pos, false, pixelColor);
            total++;
        }
    }

    std::cout << "-> " << total << " values initialized." << std::endl;
}

void RandomWalkCaveGenerator::GenerateOneStep()
{
    for (Walker * walker : m_walkers)
    {
        if (walker->StepsLeft <= 0)
        {
            continue;
        }

        int moveX = 0;
        int moveY = 0;
        while ((moveX == 0.0f && moveY == 0.0f) || moveX < 0 || moveX >= MAP_WIDTH || moveY < 0 || moveY >= MAP_HEIGHT)
        {
            // with diagonales
            // newPos.x = walker->Pos.x + GetRandomValue(-1, 1);
            // newPos.y = walker->Pos.y + GetRandomValue(-1, 1);

            // without diagonales
            int dir = GetRandomValue(0, 3);
            switch(dir)
            {
                case 0: // Right
                    moveX = 1;
                    break;
                case 1: // Up
                    moveY = -1;
                    break;
                case 2: // Left
                    moveX = -1;
                    break;
                case 3: // Down
                    moveY = 1;
                    break;
                default:
                    break;
            }
        }

        walker->PosX += moveX;
        walker->PosY += moveY;
        walker->StepsLeft--;

        floorCells[walker->PosX][walker->PosY].IsFloor();
        floorCells[walker->PosX][walker->PosY].SetFloorState(true);
    }

    PostStepCheck();
}

void RandomWalkCaveGenerator::SpawnWalker()
{
    // Random points at least 30% distance from map borders
    int randomX = GetRandomValue(MAP_WIDTH * 0.3f, MAP_WIDTH * 0.7f);
    int randomY = GetRandomValue(MAP_HEIGHT * 0.3f, MAP_HEIGHT * 0.7f);

    Walker * newWalker = new Walker {randomX, randomY, STEPS_PER_WALKER};
    m_walkers.push_back(newWalker);

    floorCells[randomX][randomY].SetFloorState(true);
}

void RandomWalkCaveGenerator::PostStepCheck()
{
    if (curStep >= timeout)
    {
        finished = true;
    }

    curStep++;
    if (curStep >= static_cast<int>(m_walkers.size()) * DELAY_BETWEEN_SPAWN && static_cast<int>(m_walkers.size()) < WALKER_NB)
    {
        SpawnWalker();
    }

    bool done = true;
    for (Walker * walker : m_walkers)
    {
        done &= walker->StepsLeft <= 0;
    }

    finished = done;
}

/// @brief Optimize the number of cells
/// @details Removes outer wall cells to reduce width and height of cells array
void RandomWalkCaveGenerator::OptimizeFloor()
{
    minX = MAP_WIDTH;
    maxX = 0;
    minY = MAP_HEIGHT;
    maxY = 0;

    bool floorFound = false;
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        floorFound = false;
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (!floorFound && floorCells[x][y].IsFloor())
            {
                floorFound = true;
                if (y < minY) minY = y;
            }

            if (floorCells[x][y].IsFloor() && y > maxY) maxY = y;
        }

        if (floorFound)
        {
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
        }
    }

    // Add 1 cell border around map edges
    // to make sure there is at least a wall on map edges
    if (minX > 0)               minX--;
    if (maxX < MAP_WIDTH - 2)   maxX++;
    if (minY > 0)               minY--;
    if (maxY < MAP_HEIGHT - 2)  maxY++;

    optimizedWidth = maxX - minX;
    optimizedHeight = maxY - minY;
}