#include <iostream>

#include "raylib.h"

#include "CellularAutomata.h"
#include "RandomWalkCaveGenerator.h"
#include <raymath.h>

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

    DisableCursor();
    
    RandomWalkCaveGenerator randomWalkMap = RandomWalkCaveGenerator();
    //randomWalkMap.FullyGenerateMap();
    randomWalkMap.PrepareMapGeneration(); 

    bool started {false};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Update
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_F5))
            started = true;

        if (IsKeyPressed(KEY_F9))
        {
            randomWalkMap.~RandomWalkCaveGenerator();
            new (&randomWalkMap) RandomWalkCaveGenerator();
            randomWalkMap.PrepareMapGeneration();
            started = false;
        }

        if (!randomWalkMap.IsFinished() && started)
            randomWalkMap.GenerateOneStep();

        // Draw
        BeginDrawing();
        ClearBackground(DARKBLUE);

            BeginMode3D(camera);
                if (started || randomWalkMap.IsFinished())
                    randomWalkMap.DrawCellMap3D();
                randomWalkMap.DrawWalkers3D();
            EndMode3D();

            // 2D Draw
            // randomWalkMap.DrawCellMap();
            // randomWalkMap.DrawWalkers();

            // if (randomWalkMap.IsFinished())
            // {
            //     randomWalkMap.DrawOptimizedMapBoundaries();
            // }

            DrawFPS(5, 5);

        EndDrawing();
    }

    CloseWindow();

    return 1;
}