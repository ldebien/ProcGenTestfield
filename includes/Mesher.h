#pragma once

#include "raylib.h"
#include <vector>

class Mesher
{
public:
    Mesher() = delete; // disallow object instance creation

    static void GenerateFloorMesh(Mesh* p_mesh, int p_subdivisions, float p_width, float p_height, Color p_color);
private:
    static void GenerateFloorMeshSubdivisionVertices(Mesh* p_mesh, Vector3 p_minPos, Vector3 p_maxPos, int p_startIdx);
    static void GenerateFloorMeshSubdivisionTexCoords(Mesh* p_mesh, Vector3 p_size, int p_startIdx);
    static void GenerateFloorMeshSubdivisionColor(Mesh* p_mesh, Color p_color, int p_startIdx);
};

/* TODO
- Mesher -> smooth edges to have a terrain look
- Mesher -> smooth edges no matter the resolution chosen
- Mesher -> Merge each cell mesh to make a single floor mesh
*/