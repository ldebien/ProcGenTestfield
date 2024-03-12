#pragma once

#include "raylib.h"
#include <vector>

class Mesher
{
public:
    Mesher() = delete; // disallow object instance creation

    static void GenerateFloorMesh(Mesh* p_mesh, int p_subdivisions, float p_width, float p_height, Color p_color);
    static void UpdateFloorMeshVertices(Mesh* p_mesh, std::vector<Vector3> p_updatedVertices);

private:
    static void GenerateFloorMeshSubdivisionVertices(Mesh* p_mesh, Vector3 p_minPos, Vector3 p_maxPos, int p_startIdx);
    static void GenerateFloorMeshSubdivisionVertexIndexes(Mesh* p_mesh, int p_startIdx, int p_vertexIdx);
    static void GenerateFloorMeshSubdivisionTexCoords(Mesh* p_mesh, Vector3 p_size, int p_startIdx);
    static void GenerateFloorMeshSubdivisionColor(Mesh* p_mesh, Color p_color, int p_startIdx);
};