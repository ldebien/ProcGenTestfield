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

void RandomWalkCaveGenerator::Init()
{
    int total = 0;
    floorPerlinNoiseImage = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 0, 0, 6.0f);

    floorCells = new Cell*[MAP_WIDTH] {};
	for(int x = 0; x < MAP_WIDTH; ++x)
    {
        floorCells[x] = new Cell[MAP_HEIGHT] {};
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            Color pixelColor = GetImageColor(floorPerlinNoiseImage, x, y);
            Vector3 pos { x + 0.5f, pixelColor.r / 32, y + 0.5f };
            floorCells[x][y].Init(pos);
            total++;
        }
    }
}

/// @brief Prepare map generation in order to generate in game loop, one step per tick
void RandomWalkCaveGenerator::PrepareMapGeneration()
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

    OptimizeFloorCells();
}

void RandomWalkCaveGenerator::GenerateOneStep()
{
    for (Walker * walker : m_walkers)
    {
        if (walker->StepsLeft <= 0)
        {
            continue;
        }

        int moveX {};
        int moveY {};
        int tries = 10;
        while ((moveX == 0.0f && moveY == 0.0f) || 
                moveX < 1 || moveX >= MAP_WIDTH - 1 || 
                moveY < 1 || moveY >= MAP_HEIGHT - 1)
        {
            moveX = 0;
            moveY = 0;
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
                    moveX = moveY = 0;
                    break;
            }

            moveX += walker->PosX;
            moveY += walker->PosY;

            tries--;
            if (tries <= 0)
            {
                std::cout << "Walker move failed. Last move candidate from " << walker->PosX << ":" << walker->PosY << " to " << moveX << ":" << moveY << std::endl;
                walker->StepsLeft = 0;
                return;
            }
        }

        walker->PosX = moveX;
        walker->PosY = moveY;
        walker->RecordedXPos.push_back(moveX);
        walker->RecordedYPos.push_back(moveY); 
        walker->StepsLeft--;

        floorCells[moveX][moveY].SetFloorType();
        floorCells[moveX][moveY].SetColor(GetImageColor(floorPerlinNoiseImage, moveX, moveY));
    }

    PostStepCheck();
}

void RandomWalkCaveGenerator::SpawnWalker()
{
    // Random points at least 30% distance from map borders
    int randomX = GetRandomValue(MAP_WIDTH * 0.3f, MAP_WIDTH * 0.7f);
    int randomY = GetRandomValue(MAP_HEIGHT * 0.3f, MAP_HEIGHT * 0.7f);

    Walker * newWalker = new Walker {randomX, randomY, STEPS_PER_WALKER};
    newWalker->RecordedXPos.push_back(randomX);
    newWalker->RecordedYPos.push_back(randomY);
    m_walkers.push_back(newWalker);

    floorCells[randomX][randomY].SetFloorType();
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

    if (finished)
    {
        OptimizeFloorCells();
    }
}

/// @brief Optimize the number of cells
/// @details Removes outer wall cells to reduce width and height of cells array
void RandomWalkCaveGenerator::OptimizeFloorCells()
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
            if (!floorFound && floorCells[x][y].GetCellType() == ECellType::Floor)
            {
                floorFound = true;
                if (y < minY) minY = y;
            }

            if (floorCells[x][y].GetCellType() == ECellType::Floor && y > maxY) maxY = y;
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
    if (maxX < MAP_WIDTH - 1)   maxX++;
    if (minY > 0)               minY--;
    if (maxY < MAP_HEIGHT - 1)  maxY++;

    optimizedWidth = maxX - minX;
    optimizedHeight = maxY - minY;

    // Set Walls & Colors
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (x >= minX && x <= maxX && y >= minY && y <= maxY)
            {
                if (floorCells[x][y].GetCellType() == ECellType::Empty)
                {
                    if (HasFloorNeighbours(x, y))
                    {
                        floorCells[x][y].SetWallType();
                        floorCells[x][y].SetColor(DARKBROWN);
                    }
                    else
                    {
                        floorCells[x][y].SetColor(BLANK);
                    }
                }
            }
            else
            {
                floorCells[x][y].SetColor(BLANK);
            }
        }
    }
}

const bool RandomWalkCaveGenerator::HasFloorNeighbours(const int x, const int y)
{
	for(int i = -1; i < 2; ++i)
    {
		for(int j = -1; j < 2; ++j)
        {
			int neighbourX = x + i;
			int neighbourY = y + j;
			if(i == 0 && j == 0 ||
                (neighbourX < 0 || neighbourY < 0 || neighbourX >= MAP_WIDTH || neighbourY >= MAP_HEIGHT))
            {
                continue;
			}
			else if(floorCells[neighbourX][neighbourY].GetCellType() == ECellType::Floor)
            {
				return true;
			}
		}
	}

    return false;
}

void RandomWalkCaveGenerator::DrawCellMap()
{
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            // if (optimizedHeight > 0 && optimizedWidth > 0 && (floorCells[x][y].GetCellType() == ECellType::Floor || floorCells[x][y].GetCellType() == ECellType::Wall))
            //     floorCells[x][y].Draw2D(Vector2Zero(), CELL_SIZE.x, CELL_SIZE.y);
            // else if (floorCells[x][y].GetCellType() == ECellType::Floor) // draw only floor tiles
                floorCells[x][y].Draw2D(Vector2Zero(), CELL_SIZE.x, CELL_SIZE.y);
            
            //DrawRectangleLines(x * CELL_SIZE.x, y * CELL_SIZE.y, CELL_SIZE.x, CELL_SIZE.y, DARKPURPLE);
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
    DrawRectangle(minX * 10.0f, minY * 10.0f, optimizedWidth * 10.0f, optimizedHeight * 10.0f, debugColor);
}

void RandomWalkCaveGenerator::DrawCellMap3D()
{
    Vector3 offset {MAP_WIDTH * -0.5f, 0.0f, MAP_HEIGHT * -0.5f};
    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            if (floorCells[x][y].GetCellType() == ECellType::Empty) continue;

            floorCells[x][y].Draw3D(offset);
        }
    }
}

void RandomWalkCaveGenerator::DrawWalkers3D()
{
    Vector3 offset {MAP_WIDTH * -0.5f, 0.0f, MAP_HEIGHT * -0.5f};
    for (Walker * walker : m_walkers)
    {
        int x = walker->PosX;
        int y = walker->PosY;
        Vector3 walkerPos {x + offset.x + 0.5f , GetImageColor(floorPerlinNoiseImage, x, y).r / 32 - offset.y + 0.75f, y + offset.z + 0.5f};
        DrawCube(walkerPos, 0.5f, 0.5f, 0.5f, GOLD);
    }
}