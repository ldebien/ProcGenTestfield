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
    return;

    std::vector<Vector3> updatedVertices {0};
    int verticesCoordCount = mesh.vertexCount * 3;
    for (int i = 0; i < verticesCoordCount; i+=3)
    {
        updatedVertices.push_back(Vector3 {mesh.vertices[i], mesh.vertices[i+1], mesh.vertices[i+2]});
    }

    std::cout << "There is " << updatedVertices.size() << " updated vertices" << std::endl;

    for (int it = 0; it < p_cornerPosY.size(); ++it)
    {
        auto cellVertice = static_cast<ECellVertice>(it);
        auto vertexIndexes = GetVertexIndex(cellVertice);
        for (int index = 0; index < vertexIndexes.size(); ++index)
        {
            updatedVertices[index].y = p_cornerPosY[cellVertice];
        }
    }

    Mesher::UpdateFloorMeshVertices(&mesh, updatedVertices);

    // for (const auto& [corner, posY] : p_cornerPosY)
    // {
    //     SetCornerPosY(corner, posY);
    // }

    //UpdateMesh();
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
    Mesher::GenerateFloorMesh(&mesh, 2, 1.0f, 1.0f, color);
    UploadMesh(&mesh, false);

    model = LoadModelFromMesh(mesh);
}

void Cell::UpdateMesh()
{
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

    // int vertexCount {12};

    // Vector3 *vertices = new Vector3[vertexCount] {};
    // *vertices = properties->cornerPos[ECellVertice::TopLeft];
    // *(vertices+1) = properties->cornerPos[ECellVertice::Left];
    // *(vertices+2) = properties->cornerPos[ECellVertice::Top];
    // *(vertices+3) = correctedCenter;
    // *(vertices+4) = properties->cornerPos[ECellVertice::TopRight];
    // *(vertices+5) = properties->cornerPos[ECellVertice::Right];
    // *(vertices+6) = properties->cornerPos[ECellVertice::Left]; 
    // *(vertices+7) = properties->cornerPos[ECellVertice::BotLeft];
    // *(vertices+8) = correctedCenter;
    // *(vertices+9) = properties->cornerPos[ECellVertice::Bot];
    // *(vertices+10) = properties->cornerPos[ECellVertice::Right];
    // *(vertices+11) = properties->cornerPos[ECellVertice::BotRight];

    // float *floatVertices = new float[3] {};
    // for (int i = 0; i < vertexCount * 3; i+=3)
    // {
    //     floatVertices[i] = vertices[i/3].x;
    //     floatVertices[i+1] = vertices[i/3].y;
    //     floatVertices[i+2] = vertices[i/3].z;
    // }
    
    //UpdateMeshBuffer(*cellMesh, 0, floatVertices, vertexCount * 3, 0);
}

const std::vector<int> Cell::GetVertexIndex(ECellVertice p_vertex)
{
    /*
    0- top left
    1- bot left
    2- top right
    3- top right
    4- bot left
    5- bot right
    */
    
    int verticesPerSubdiv = 6;
    std::vector<int> indexes {0};

    switch (p_vertex)
    {
        case ECellVertice::TopLeft:
            indexes.push_back(0); // OK
            break;
        case ECellVertice::Left:
            indexes.push_back(1); // OK
            indexes.push_back(4); // OK
            indexes.push_back(verticesPerSubdiv * 2); // OK
            break;
        case ECellVertice::Top:
            indexes.push_back(2); // OK
            indexes.push_back(3); // OK
            indexes.push_back(verticesPerSubdiv); // OK
            break;
        case ECellVertice::TopRight:
            indexes.push_back(verticesPerSubdiv + 2); // OK
            indexes.push_back(verticesPerSubdiv + 3); // OK
            break;
        case ECellVertice::Right:
            indexes.push_back(verticesPerSubdiv + 5);  // OK
            indexes.push_back(verticesPerSubdiv * 3 + 2); // OK
            indexes.push_back(verticesPerSubdiv * 3 + 3); // OK
            break;
        case ECellVertice::BotLeft:
            indexes.push_back(verticesPerSubdiv * 2 + 1); // OK
            indexes.push_back(verticesPerSubdiv * 2 + 4); // OK
            break;
        case ECellVertice::Bot:
            indexes.push_back(verticesPerSubdiv * 2 + 5); // OK
            indexes.push_back(verticesPerSubdiv * 3 + 1); // OK
            indexes.push_back(verticesPerSubdiv * 3 + 4); // OK
            break;
        case ECellVertice::BotRight:
            indexes.push_back(verticesPerSubdiv * 3 + 5); // OK
            break;
        case ECellVertice::Center:
            indexes.push_back(5); // OK
            indexes.push_back(verticesPerSubdiv + 1); // OK
            indexes.push_back(verticesPerSubdiv + 4); // OK
            indexes.push_back(verticesPerSubdiv * 2 + 2); // OK
            indexes.push_back(verticesPerSubdiv * 2 + 3); // OK
            indexes.push_back(verticesPerSubdiv * 3); // OK
            break;
        default:
            break;
    }

    return indexes;
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