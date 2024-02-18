#include <iostream>

#include "raylib.h"

#include "CellularAutomata.h"
#include "RandomWalkCaveGenerator.h"

int main()
{
    const int winWidth = 1280;
    const int winHeight = 720;

    InitWindow(winWidth, winHeight, "Proc Gen Testfield");

    Camera3D camera = { 0 };
    camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //DisableCursor();

    //CellularAutomata cellularAutomataMap = CellularAutomata();
    //     for (int i = 0; i < 4; ++i)
    //     {
    //         cellularAutomataMap.DoSimulationStep(true);
    //     }
    //     for (int i = 0; i < 3; ++i)
    //     {
    //         cellularAutomataMap.DoSimulationStep(false);
    //     }
    RandomWalkCaveGenerator randomWalkMap = RandomWalkCaveGenerator();
    //randomWalkMap.FullyGenerateMap();
    randomWalkMap.PrepareMapGeneration();

    bool started {false};

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        // Update
        //UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_F5))
            started = true;

        if (IsKeyPressed(KEY_F9))
        {
            randomWalkMap.~RandomWalkCaveGenerator();
            new (&randomWalkMap) RandomWalkCaveGenerator();
            randomWalkMap.PrepareMapGeneration();
        }

        // if (IsKeyPressed(KEY_F9))
        // {
        //     randomWalkMap.~RandomWalkCaveGenerator();
        //     new (&randomWalkMap) RandomWalkCaveGenerator();
        //     randomWalkMap.PrepareMapGeneration();
        // }
                
        // if (IsKeyPressed(KEY_F9))
        // {
        //     cellularAutomataMap.~CellularAutomata();
        //     new (&cellularAutomataMap) CellularAutomata();
        // }
        // if (IsKeyPressed(KEY_F10))
        // {
        //     cellularAutomataMap.~CellularAutomata();
        //     new (&cellularAutomataMap) CellularAutomata();
        //     for (int i = 0; i < 4; ++i)
        //     {
        //         cellularAutomataMap.DoSimulationStep(true);
        //     }
        //     for (int i = 0; i < 3; ++i)
        //     {
        //         cellularAutomataMap.DoSimulationStep(false);
        //     }
        // }
        // if (IsKeyPressed(KEY_F11))
        // {
        //     cellularAutomataMap.DoSimulationStep(true);
        // }
        // if (IsKeyPressed(KEY_F12))
        // {
        //     cellularAutomataMap.DoSimulationStep(false);
        // }

        if (!randomWalkMap.IsFinished() && started)
            randomWalkMap.GenerateOneStep();

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

            //BeginMode3D(camera);
                    // randomWalkMap.DrawCellMap3D();
                    // randomWalkMap.DrawWalkers3D();

                    //DrawGrid(1300, 1.0f);
            //EndMode3D();

            //cellularAutomataMap.DrawCellMap();
            randomWalkMap.DrawCellMap();
            randomWalkMap.DrawWalkers();

            DrawFPS(5, 5);

        EndDrawing();
    }

    CloseWindow();

    return 1;
}