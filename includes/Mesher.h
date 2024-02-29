#pragma once

#include "raylib.h"

class Mesher
{
public:
    Mesher() = delete; // disallow object instance creation

    static void GenerateFloorMesh(Mesh* p_mesh, int p_subdivisions, float p_width, float p_height);
private:
    static void GenerateFloorMeshSubdivision(Mesh* p_mesh, Vector3 p_minPos, Vector3 p_maxPos, Vector3 p_size, int p_vertexStartIdx, Color p_color);
};