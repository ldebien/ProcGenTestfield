#include "includes/Mesher.h"

#include "raymath.h"
#include <iostream>

void Mesher::GenerateFloorMesh(Mesh* p_mesh, int p_definition, float p_width, float p_height, Color p_color)
{
    int subdivNb = p_definition * p_definition;

    p_mesh->triangleCount = 2 * subdivNb;
    p_mesh->vertexCount = p_mesh->triangleCount * 3;
    p_mesh->vertices = (float *)MemAlloc(p_mesh->vertexCount * 3 * sizeof(float));                    // 3 vertices, 3 coordinates each (x, y, z)
    p_mesh->texcoords = (float *)MemAlloc(p_mesh->vertexCount * 2 * sizeof(float));                   // 3 vertices, 2 coordinates each (x, y)
    p_mesh->normals = (float *)MemAlloc(p_mesh->vertexCount * 3 * sizeof(float));                     // 3 vertices, 3 coordinates each (x, y, z)
    p_mesh->colors = (unsigned char *)MemAlloc(p_mesh->vertexCount * 4 * sizeof(unsigned char));      // 3 vertices, 4 color components (r, g, b, a)

    Vector3 subdivSize = { p_width / p_definition, 0.0f, p_height / p_definition };
    Vector3 offset = { p_width * -0.5f, 0.0f, p_height * -0.5f }; // half size offset to have pivot in mesh center (which looks like a quad facing up with y = 0)

    Vector3 subdivPosMin { 0 }; 
    Vector3 subdivPosMax { 0 };
    int verticeCoordPerSubdiv = p_mesh->vertexCount * 3 / subdivNb;
    int texCoordPerSubdiv = p_mesh->vertexCount * 2 / subdivNb;
    int colorCompPerSubdiv = p_mesh->vertexCount * 4 / subdivNb;

    int vertexIdx = 0;
    int texCoordIdx = 0;
    int colorCompIdx = 0;
    for (int x = 0; x < p_definition; ++x)
    {
        subdivPosMin = { x * subdivSize.x + offset.x, subdivSize.y + offset.y, 0.0f };
        for (int z = 0; z < p_definition; ++z)
        {
            subdivPosMin.z = z * subdivSize.z + offset.z;
            subdivPosMax = Vector3Add(subdivPosMin, subdivSize);

            GenerateFloorMeshSubdivisionVertices(p_mesh, subdivPosMin, subdivPosMax, vertexIdx);
            GenerateFloorMeshSubdivisionTexCoords(p_mesh, subdivSize, texCoordIdx);
            GenerateFloorMeshSubdivisionColor(p_mesh, p_color, colorCompIdx);

            vertexIdx += verticeCoordPerSubdiv;
            texCoordIdx += texCoordPerSubdiv;
            colorCompIdx += colorCompPerSubdiv;
        }
    }
}

void Mesher::GenerateFloorMeshSubdivisionVertices(Mesh* p_mesh, Vector3 p_minPos, Vector3 p_maxPos, int p_startIdx)
{
    // Top Left Vertex
    p_mesh->vertices[p_startIdx+0] = p_minPos.x;
    p_mesh->vertices[p_startIdx+1] = 0.0f;
    p_mesh->vertices[p_startIdx+2] = p_minPos.z;
    p_mesh->normals[p_startIdx+0] = 0.0f;
    p_mesh->normals[p_startIdx+1] = 1.0f;
    p_mesh->normals[p_startIdx+2] = 0.0f;

    // Bottom Left Vertex
    p_mesh->vertices[p_startIdx+3] = p_minPos.x;
    p_mesh->vertices[p_startIdx+4] = 0.0f;
    p_mesh->vertices[p_startIdx+5] = p_maxPos.z;
    p_mesh->normals[p_startIdx+3] = 0.0f;
    p_mesh->normals[p_startIdx+4] = 1.0f;
    p_mesh->normals[p_startIdx+5] = 0.0f;

    // Top Right Vertex
    p_mesh->vertices[p_startIdx+6] = p_maxPos.x;
    p_mesh->vertices[p_startIdx+7] = 0.0f;
    p_mesh->vertices[p_startIdx+8] = p_minPos.z;
    p_mesh->normals[p_startIdx+6] = 0.0f;
    p_mesh->normals[p_startIdx+7] = 1.0f;
    p_mesh->normals[p_startIdx+8] = 0.0f;

    // Top Right Vertex
    p_mesh->vertices[p_startIdx+9] = p_maxPos.x;
    p_mesh->vertices[p_startIdx+10] = 0.0f;
    p_mesh->vertices[p_startIdx+11] = p_minPos.z;
    p_mesh->normals[p_startIdx+9] = 0.0f;
    p_mesh->normals[p_startIdx+10] = 1.0f;
    p_mesh->normals[p_startIdx+11] = 0.0f;

    // Bottom Left Vertex
    p_mesh->vertices[p_startIdx+12] = p_minPos.x;
    p_mesh->vertices[p_startIdx+13] = 0.0f;
    p_mesh->vertices[p_startIdx+14] = p_maxPos.z;
    p_mesh->normals[p_startIdx+12] = 0.0f;
    p_mesh->normals[p_startIdx+13] = 1.0f;
    p_mesh->normals[p_startIdx+14] = 0.0f;

    // Bottom Right Vertex
    p_mesh->vertices[p_startIdx+15] = p_maxPos.x;
    p_mesh->vertices[p_startIdx+16] = 0.0f;
    p_mesh->vertices[p_startIdx+17] = p_maxPos.z;
    p_mesh->normals[p_startIdx+15] = 0.0f;
    p_mesh->normals[p_startIdx+16] = 1.0f;
    p_mesh->normals[p_startIdx+17] = 0.0f;
}

void Mesher::GenerateFloorMeshSubdivisionTexCoords(Mesh* p_mesh, Vector3 p_size, int p_startIdx)
{
    // Top Left Vertex
    p_mesh->texcoords[p_startIdx+0] = 0.0f;
    p_mesh->texcoords[p_startIdx+1] = p_size.z;

    // Bottom Left Vertex
    p_mesh->texcoords[p_startIdx+2] = 0.0f;
    p_mesh->texcoords[p_startIdx+3] = 0.0f;

    // Top Right Vertex
    p_mesh->texcoords[p_startIdx+4] = p_size.x;
    p_mesh->texcoords[p_startIdx+5] = p_size.z;

    // Top Right Vertex
    p_mesh->texcoords[p_startIdx+6] = p_size.x;
    p_mesh->texcoords[p_startIdx+7] = p_size.z;

    // Bottom Left Vertex
    p_mesh->texcoords[p_startIdx+8] = 0.0f;
    p_mesh->texcoords[p_startIdx+9] = 0.0f;

    // Bottom Right Vertex
    p_mesh->texcoords[p_startIdx+10] = p_size.x;
    p_mesh->texcoords[p_startIdx+11] = 0.0f;
}

void Mesher::GenerateFloorMeshSubdivisionColor(Mesh* p_mesh, Color p_color, int p_startIdx)
{
    int max = p_startIdx + 6 * 4; // 4 color component per vertex of a subdiv
    for (int i = p_startIdx; i < max; i+=4)
    {
        p_mesh->colors[i+0] = p_color.r;
        p_mesh->colors[i+1] = p_color.g;
        p_mesh->colors[i+2] = p_color.b;
        p_mesh->colors[i+3] = p_color.a;
    }
}