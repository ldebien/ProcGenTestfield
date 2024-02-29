#include "includes/Mesher.h"

#include "raymath.h"
#include <iostream>

void Mesher::GenerateFloorMesh(Mesh* p_mesh, int p_definition, float p_width, float p_height)
{
    p_mesh->triangleCount = 2 * (p_definition * 2);
    p_mesh->vertexCount = p_mesh->triangleCount*3;
    p_mesh->vertices = (float *)MemAlloc(p_mesh->vertexCount*3*sizeof(float));                    // 3 vertices, 3 coordinates each (x, y, z)
    p_mesh->texcoords = (float *)MemAlloc(p_mesh->vertexCount*2*sizeof(float));                   // 3 vertices, 2 coordinates each (x, y)
    p_mesh->normals = (float *)MemAlloc(p_mesh->vertexCount*3*sizeof(float));                     // 3 vertices, 3 coordinates each (x, y, z)
    p_mesh->colors = (unsigned char *)MemAlloc(p_mesh->vertexCount*4*sizeof(unsigned char));      // 3 vertices, 4 color components (r, g, b, a)

    Vector3 subdivSize = { p_width / p_definition, 0.0f, p_height / p_definition };
    Vector3 offset = { p_width * -0.5f, 0.0f, p_height * -0.5f }; // half size offset to have pivot in mesh center (which looks like a quad facing up with y = 0)

    Vector3 subdivPosMin { 0 }; 
    Vector3 subdivPosMax { 0 };
    int verticePerSubdiv = p_mesh->vertexCount / (p_definition * 2);
    Color subdivColor = RAYWHITE;

    int vertexIdx = 0; 
    for (int x = 0; x < p_definition; ++x)
    {
        subdivPosMin = { x * subdivSize.x + offset.x, subdivSize.y + offset.y, 0.0f };
        for (int z = 0; z < p_definition; ++z)
        {
            subdivPosMin.z = z * subdivSize.z + offset.z;
            subdivPosMax = Vector3Add(subdivPosMin, subdivSize);
            subdivColor = x % 2 == z % 2 ? RAYWHITE : GRAY;

            GenerateFloorMeshSubdivision(p_mesh, subdivPosMin, subdivPosMax, subdivSize, vertexIdx, subdivColor);

            vertexIdx += verticePerSubdiv;
        }
    }
}

void Mesher::GenerateFloorMeshSubdivision(Mesh* p_mesh, Vector3 p_minPos, Vector3 p_maxPos, Vector3 p_size, int p_vertexStartIdx, Color p_color)
{
    std::cout << "Start index " << p_vertexStartIdx << " | min: " << p_minPos.x << ";" << p_minPos.z << " & max: " << p_maxPos.x << ";" << p_maxPos.z <<  std::endl;

    // Top Left Vertex
    p_mesh->vertices[p_vertexStartIdx+0] = p_minPos.x;
    p_mesh->vertices[p_vertexStartIdx+1] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+2] = p_minPos.z;
    p_mesh->normals[p_vertexStartIdx+0] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+1] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+2] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+0] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+1] = p_size.z;

    // // Top Left Vertex
    // mesh.vertices[0] = -0.5f;
    // mesh.vertices[1] = 0.0f;
    // mesh.vertices[2] = -0.5f;
    // mesh.normals[0] = 0.0f;
    // mesh.normals[1] = 1.0f;
    // mesh.normals[2] = 0.0f;
    // mesh.texcoords[0] = 0.0f;
    // mesh.texcoords[1] = 1.0f;

    // Bottom Left Vertex
    p_mesh->vertices[p_vertexStartIdx+3] = p_minPos.x;
    p_mesh->vertices[p_vertexStartIdx+4] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+5] = p_maxPos.z;
    p_mesh->normals[p_vertexStartIdx+3] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+4] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+5] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+2] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+3] = 0.0f;

    // // Bottom Left Vertex
    // mesh.vertices[3] = -0.5f;
    // mesh.vertices[4] = 0.0f;
    // mesh.vertices[5] = 0.5f;
    // mesh.normals[3] = 0.0f;
    // mesh.normals[4] = 1.0f;
    // mesh.normals[5] = 0.0f;
    // mesh.texcoords[2] = 0.0f;
    // mesh.texcoords[3] = 0.0f;

    // Top Right Vertex
    p_mesh->vertices[p_vertexStartIdx+6] = p_maxPos.x;
    p_mesh->vertices[p_vertexStartIdx+7] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+8] = p_minPos.z;
    p_mesh->normals[p_vertexStartIdx+6] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+7] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+8] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+4] = p_size.x;
    p_mesh->texcoords[p_vertexStartIdx+5] = p_size.z;

    // // Top Right Vertex
    // mesh.vertices[9] = 0.5f;
    // mesh.vertices[10] = 0.0f;
    // mesh.vertices[11] = -0.5f;
    // mesh.normals[9] = 0.0f;
    // mesh.normals[10] = 1.0f;
    // mesh.normals[11] = 0.0f;
    // mesh.texcoords[6] = 1.0f;
    // mesh.texcoords[7] = 1.0f;

    // Top Right Vertex
    p_mesh->vertices[p_vertexStartIdx+9] = p_maxPos.x;
    p_mesh->vertices[p_vertexStartIdx+10] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+11] = p_minPos.z;
    p_mesh->normals[p_vertexStartIdx+9] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+10] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+11] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+6] = p_size.x;
    p_mesh->texcoords[p_vertexStartIdx+7] = p_size.z;
    
    // // Top Right Vertex
    // mesh.vertices[9] = 0.5f;
    // mesh.vertices[10] = 0.0f;
    // mesh.vertices[11] = -0.5f;
    // mesh.normals[9] = 0.0f;
    // mesh.normals[10] = 1.0f;
    // mesh.normals[11] = 0.0f;
    // mesh.texcoords[6] = 1.0f;
    // mesh.texcoords[7] = 1.0f;

    // Bottom Left Vertex
    p_mesh->vertices[p_vertexStartIdx+12] = p_minPos.x;
    p_mesh->vertices[p_vertexStartIdx+13] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+14] = p_maxPos.z;
    p_mesh->normals[p_vertexStartIdx+12] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+13] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+14] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+8] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+9] = 0.0f;

    // // Bottom Left Vertex
    // mesh.vertices[12] = -0.5f;
    // mesh.vertices[13] = 0.0f;
    // mesh.vertices[14] = 0.5f;
    // mesh.normals[12] = 0.0f;
    // mesh.normals[13] = 1.0f;
    // mesh.normals[14] = 0.0f;
    // mesh.texcoords[8] = 0.0f;
    // mesh.texcoords[9] = 0.0f;

    // Bottom Right Vertex
    p_mesh->vertices[p_vertexStartIdx+15] = p_maxPos.x;
    p_mesh->vertices[p_vertexStartIdx+16] = 0.0f;
    p_mesh->vertices[p_vertexStartIdx+17] = p_maxPos.z;
    p_mesh->normals[p_vertexStartIdx+15] = 0.0f;
    p_mesh->normals[p_vertexStartIdx+16] = 1.0f;
    p_mesh->normals[p_vertexStartIdx+17] = 0.0f;
    p_mesh->texcoords[p_vertexStartIdx+10] = p_size.x;
    p_mesh->texcoords[p_vertexStartIdx+11] = 0.0f;

    // // Bottom Right Vertex
    // mesh.vertices[15] = 0.5f;
    // mesh.vertices[16] = 0.0f;
    // mesh.vertices[17] = 0.5f;
    // mesh.normals[15] = 0.0f;
    // mesh.normals[16] = 1.0f;
    // mesh.normals[17] = 0.0f;
    // mesh.texcoords[10] = 1.0f;
    // mesh.texcoords[11] = 0.0f;

    for (int i = 0; i < p_mesh->vertexCount*4; i+=4)
    {
        p_mesh->colors[p_vertexStartIdx+i+0] = p_color.r;
        p_mesh->colors[p_vertexStartIdx+i+1] = p_color.g;
        p_mesh->colors[p_vertexStartIdx+i+2] = p_color.b;
        p_mesh->colors[p_vertexStartIdx+i+3] = p_color.a;
    }
}