#include "Cell.h"

#include "raymath.h"
#include <iostream>

#include "includes/Mesher.h"

Cell::Cell()
{
    properties = new CellProperties
    {
        ECellType::Empty,
        Vector3Zero(),
        {
            {ECellVertice::Right,       {0.5f, 0.0f, 0.0f}},
            {ECellVertice::TopRight,    {0.5f, 0.0f, -0.5f}},
            {ECellVertice::Top,         {0.0f, 0.0f, -0.5f}},
            {ECellVertice::TopLeft,     {-0.5f, 0.0f, -0.5f}},
            {ECellVertice::Left,        {-0.5f, 0.0f, 0.0f}},
            {ECellVertice::BotLeft,     {-0.5f, 0.0f, 0.5f}},
            {ECellVertice::Bot,         {0.0f, 0.0f, 0.5f}},
            {ECellVertice::BotRight,    {0.5f, 0.0f, 0.5f}}
        }
    };
}

Cell::~Cell()
{
    if (properties != nullptr) delete properties;
}

void Cell::Init(Vector3 p_pos)
{
    properties->centerPos = p_pos;
    // properties->cornerPos =
    // {
    //     {ECellVertice::Right,       Vector3Add(p_pos, {0.5f, 0.0f, 0.0f})},
    //     {ECellVertice::TopRight,    Vector3Add(p_pos, {0.5f, 0.0f, -0.5f})},
    //     {ECellVertice::Top,         Vector3Add(p_pos, {0.0f, 0.0f, -0.5f})},
    //     {ECellVertice::TopLeft,     Vector3Add(p_pos, {-0.5f, 0.0f, -0.5f})},
    //     {ECellVertice::Left,        Vector3Add(p_pos, {-0.5f, 0.0f, 0.0f})},
    //     {ECellVertice::BotLeft,     Vector3Add(p_pos, {-0.5f, 0.0f, 0.5f})},
    //     {ECellVertice::Bot,         Vector3Add(p_pos, {0.0f, 0.0f, 0.5f})},
    //     {ECellVertice::BotRight,    Vector3Add(p_pos, {0.5f, 0.0f, 0.5f})}
    // };

    color = BLANK;
}

void Cell::SetCornersPosY(std::map<ECellVertice, float> p_cornerPosY)
{
    for (const auto& [corner, posY] : p_cornerPosY)
    {
        SetCornerPosY(corner, posY);
    }

    UpdateMesh();
}

void Cell::SetType(ECellType p_newType)
{
    if (p_newType == properties->type) return;

    properties->type = p_newType;
    GenerateMesh();
}

void Cell::SetCornerPosY(ECellVertice p_corner, float p_posY)
{
    properties->cornerPos[p_corner].y = p_posY;
}

void Cell::GenerateMesh()
{
    switch (properties->type)
    {
        case ECellType::Floor:
            GenerateFloorMesh2();
            break;
        case ECellType::Wall:
        case ECellType::Empty:
        default:
            break;
    }
}

void Cell::GenerateFloorMesh()
{
    int vertexCount {12};

    Vector3 *vertices = new Vector3[vertexCount] {};
    *vertices = properties->cornerPos[ECellVertice::TopLeft];
    *(vertices+1) = properties->cornerPos[ECellVertice::Left];
    *(vertices+2) = properties->cornerPos[ECellVertice::Top];
    *(vertices+3) = properties->centerPos;
    *(vertices+4) = properties->cornerPos[ECellVertice::TopRight];
    *(vertices+5) = properties->cornerPos[ECellVertice::Right];
    *(vertices+6) = properties->cornerPos[ECellVertice::Left];
    *(vertices+7) = properties->cornerPos[ECellVertice::BotLeft];
    *(vertices+8) = properties->centerPos;
    *(vertices+9) = properties->cornerPos[ECellVertice::Bot];
    *(vertices+10) = properties->cornerPos[ECellVertice::Right];
    *(vertices+11) = properties->cornerPos[ECellVertice::BotRight];

    // It displays properly with DrawTriangleStrip3D function
    //DrawTriangleStrip3D(vertices, vertexCount, color);

    if (cellMesh == nullptr) cellMesh = new Mesh {};

    cellMesh->vertexCount = vertexCount;
    cellMesh->triangleCount = 8;

    if (cellMesh->vertices != nullptr) delete[] cellMesh->vertices;
    if (cellMesh->normals != nullptr) delete[] cellMesh->normals;
    cellMesh->vertices = new float[vertexCount * 3];
    cellMesh->normals = new float[vertexCount * 3];
    for (int i = 0; i < vertexCount * 3; i+=3)
    {
        cellMesh->vertices[i] = vertices[i/3].x;
        cellMesh->vertices[i+1] = vertices[i/3].y;
        cellMesh->vertices[i+2] = vertices[i/3].z;

        cellMesh->normals[i] = 0.0f;
        cellMesh->normals[i+1] = 1.0f;
        cellMesh->normals[i+2] = 0.0f;
    }

    if (cellMesh->colors != nullptr) delete[] cellMesh->colors;
    cellMesh->colors = new unsigned char[vertexCount * 4];
    for (int i = 0; i < vertexCount * 4; i+=4)
    {
        cellMesh->colors[i] = color.r;
        cellMesh->colors[i+1] = color.g;
        cellMesh->colors[i+2] = color.b;
        cellMesh->colors[i+3] = color.a;
    }

    if (cellMesh->texcoords != nullptr) delete[] cellMesh->texcoords;
    if (cellMesh->texcoords2 != nullptr) delete[] cellMesh->texcoords2;
    cellMesh->texcoords = new float[vertexCount * 2];
    cellMesh->texcoords2 = new float[vertexCount * 2];
    for (int i = 0; i < vertexCount * 2; i+=2)
    {
        cellMesh->texcoords[i] = 0.0f;
        cellMesh->texcoords[i+1] = 1.0f;

        cellMesh->texcoords2[i] = 0.0f;
        cellMesh->texcoords2[i+1] = 1.0f;
    }

    if (cellMesh->indices != nullptr) delete[] cellMesh->indices;
    cellMesh->indices = new unsigned short[12];
    for (unsigned short i = 0; i < vertexCount; ++i)
    {
        cellMesh->indices[i] = i;
    }

    if (cellMesh->animVertices == nullptr)
    {
        cellMesh->animVertices = new float[0];
        cellMesh->animNormals = new float[0];
        cellMesh->boneIds = new unsigned char[0];
        cellMesh->boneWeights = new float[0];
    }

    GenMeshTangents(cellMesh);

    if (cellMesh->vboId != nullptr)
    {
        std::cout << "already exists" << std::endl;
    }

    UploadMesh(cellMesh, false);
    
    delete[] vertices;
}

void Cell::GenerateFloorMesh2()
{
    // mesh = { 0 };
    // mesh.triangleCount = 2;
    // mesh.vertexCount = mesh.triangleCount*3;
    // mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));                    // 3 vertices, 3 coordinates each (x, y, z)
    // mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));                   // 3 vertices, 2 coordinates each (x, y)
    // mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));                     // 3 vertices, 3 coordinates each (x, y, z)
    // mesh.colors = (unsigned char *)MemAlloc(mesh.vertexCount*4*sizeof(unsigned char));      // 3 vertices, 4 color components (r, g, b, a)

    // // Top Left Vertex
    // mesh.vertices[0] = -0.5f;
    // mesh.vertices[1] = 0.0f;
    // mesh.vertices[2] = -0.5f;
    // mesh.normals[0] = 0.0f;
    // mesh.normals[1] = 1.0f;
    // mesh.normals[2] = 0.0f;
    // mesh.texcoords[0] = 0.0f;
    // mesh.texcoords[1] = 1.0f;

    // // Bottom Left Vertex
    // mesh.vertices[3] = -0.5f;
    // mesh.vertices[4] = 0.0f;
    // mesh.vertices[5] = 0.5f;
    // mesh.normals[3] = 0.0f;
    // mesh.normals[4] = 1.0f;
    // mesh.normals[5] = 0.0f;
    // mesh.texcoords[2] = 0.0f;
    // mesh.texcoords[3] = 0.0f;

    // // Top Right Vertex
    // mesh.vertices[6] = 0.5f;
    // mesh.vertices[7] = 0.0f;
    // mesh.vertices[8] = -0.5f;
    // mesh.normals[6] = 0.0f;
    // mesh.normals[7] = 1.0f;
    // mesh.normals[8] = 0.0f;
    // mesh.texcoords[4] = 1.0f;
    // mesh.texcoords[5] = 1.0f;

    // // Top Right Vertex
    // mesh.vertices[9] = 0.5f;
    // mesh.vertices[10] = 0.0f;
    // mesh.vertices[11] = -0.5f;
    // mesh.normals[9] = 0.0f;
    // mesh.normals[10] = 1.0f;
    // mesh.normals[11] = 0.0f;
    // mesh.texcoords[6] = 1.0f;
    // mesh.texcoords[7] = 1.0f;

    // // Bottom Left Vertex
    // mesh.vertices[12] = -0.5f;
    // mesh.vertices[13] = 0.0f;
    // mesh.vertices[14] = 0.5f;
    // mesh.normals[12] = 0.0f;
    // mesh.normals[13] = 1.0f;
    // mesh.normals[14] = 0.0f;
    // mesh.texcoords[8] = 0.0f;
    // mesh.texcoords[9] = 0.0f;

    // // Bottom Right Vertex
    // mesh.vertices[15] = 0.5f;
    // mesh.vertices[16] = 0.0f;
    // mesh.vertices[17] = 0.5f;
    // mesh.normals[15] = 0.0f;
    // mesh.normals[16] = 1.0f;
    // mesh.normals[17] = 0.0f;
    // mesh.texcoords[10] = 1.0f;
    // mesh.texcoords[11] = 0.0f;

    // // All vertices are the same color
    // for (int i = 0; i < mesh.vertexCount*4; i+=4)
    // {
    //     mesh.colors[i] = color.r;
    //     mesh.colors[i+1] = color.g;
    //     mesh.colors[i+2] = color.b;
    //     mesh.colors[i+3] = color.a;
    // }

    Mesher::GenerateFloorMesh(&mesh, 1, 1.0f, 1.0f);
    std::cout << "Cell " << properties->centerPos.x << "x" << properties->centerPos.z << "(" << mesh.vertexCount << ") vertices:" << std::endl;
    for (int i = 0; i < mesh.vertexCount*3; i+=3)
    {
        std::cout << "\t[" << mesh.vertices[i] << ";" << mesh.vertices[i+1] << ";" << mesh.vertices[i+2] << "]" << std::endl;
    }

    UploadMesh(&mesh, false);

    model = LoadModelFromMesh(mesh);
}

void Cell::UpdateMesh()
{
    return;
    switch (properties->type)
    {
        case ECellType::Floor:
            UpdateFloorMeshHeight();
            break;
        case ECellType::Wall:
        case ECellType::Empty:
        default:
            break;
    }
}

void Cell::UpdateFloorMeshHeight()
{
    float correctedY = 0;
    for (auto const& [corner, pos] : properties->cornerPos)
    {
        correctedY += pos.y;
    }

    Vector3 correctedCenter = properties->centerPos;
    correctedCenter.y = correctedY / properties->cornerPos.size();

    int vertexCount {12};

    Vector3 *vertices = new Vector3[vertexCount] {};
    *vertices = properties->cornerPos[ECellVertice::TopLeft];
    *(vertices+1) = properties->cornerPos[ECellVertice::Left];
    *(vertices+2) = properties->cornerPos[ECellVertice::Top];
    *(vertices+3) = correctedCenter;
    *(vertices+4) = properties->cornerPos[ECellVertice::TopRight];
    *(vertices+5) = properties->cornerPos[ECellVertice::Right];
    *(vertices+6) = properties->cornerPos[ECellVertice::Left]; 
    *(vertices+7) = properties->cornerPos[ECellVertice::BotLeft];
    *(vertices+8) = correctedCenter;
    *(vertices+9) = properties->cornerPos[ECellVertice::Bot];
    *(vertices+10) = properties->cornerPos[ECellVertice::Right];
    *(vertices+11) = properties->cornerPos[ECellVertice::BotRight];

    float *floatVertices = new float[3] {};
    for (int i = 0; i < vertexCount * 3; i+=3)
    {
        floatVertices[i] = vertices[i/3].x;
        floatVertices[i+1] = vertices[i/3].y;
        floatVertices[i+2] = vertices[i/3].z;
    }
    
    UpdateMeshBuffer(*cellMesh, 0, floatVertices, vertexCount * 3, 0);
}

const void Cell::Draw3D(Vector3 p_offset)
{
    // Vector3 size = Vector3One();
    // //Color color = WHITE;
    // if (properties->type == ECellType::Wall)
    // {
    //     p_offset.y += 3.0f;
    //     size.y = 6.0f;
    //     //color = DARKBROWN;
    // }

    // Vector3 pos = Vector3Add(properties->centerPos, p_offset);
    // DrawCubeV(pos, size, color);

    // if (model == nullptr)
    // {
    //     return;
    // }
    
    // DrawModel(*model, p_offset, 1.0f, color);

    //DrawMesh(*cellMesh, LoadMaterialDefault(), MatrixIdentity());
    // Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
    // Matrix matRotation = MatrixRotate({0.0f, 1.0f, 0.0f}, 0.0f*DEG2RAD);

    // Matrix matScale = MatrixIdentity();
    // Matrix matRotation = MatrixIdentity();
    // Matrix matTranslation = MatrixTranslate(properties->centerPos.x, properties->centerPos.y, properties->centerPos.z);
    // Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

    // DrawMesh(mesh, LoadMaterialDefault(), MatrixMultiply(MatrixIdentity(), matTransform));
    // model = LoadModelFromMesh(mesh);

    DrawModel(model, Vector3Add(properties->centerPos, p_offset), 1.0f, WHITE);
    //DrawCube(properties->centerPos, 0.2f, 3.0f, 0.2f, RED);

}

const void Cell::Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight)
{
    Vector2 centerPos = GetPos2D();
    int posX = centerPos.x + p_offset.x;
    int posY = centerPos.y + p_offset.y;

    DrawRectangle(posX * p_cellWidth, posY * p_cellHeight, p_cellWidth, p_cellHeight, color);
}