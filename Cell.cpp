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
            GenerateFloorMesh();
            break;
        case ECellType::Wall:
        case ECellType::Empty:
        default:
            break;
    }
}

void Cell::GenerateFloorMesh()
{
    Mesher::GenerateFloorMesh(&mesh, 2, 1.0f, 1.0f);
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
    DrawModel(model, Vector3Add(properties->centerPos, p_offset), 1.0f, WHITE);
}

const void Cell::Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight)
{
    Vector2 centerPos = GetPos2D();
    int posX = centerPos.x + p_offset.x;
    int posY = centerPos.y + p_offset.y;

    DrawRectangle(posX * p_cellWidth, posY * p_cellHeight, p_cellWidth, p_cellHeight, color);
}