#include "Cell.h"


#include "raymath.h"

Cell::Cell()
{
    properties = new CellProperties
    {
        false,
        Vector3Zero(),
        {
            {ECorner::TopRight, Vector3Zero()},
            {ECorner::TopLeft, Vector3Zero()},
            {ECorner::BotRight, Vector3Zero()},
            {ECorner::BotLeft, Vector3Zero()}
        }
    };
}

Cell::~Cell()
{
    if (properties != nullptr) delete properties;
}

void Cell::Init(Vector3 p_pos, bool p_floorState, Color p_color)
{
    properties->isFloor = p_floorState;

    // Flat floor/tile
    properties->centerPos = p_pos;
    properties->cornerPos =
    {
        {ECorner::TopRight, p_pos},
        {ECorner::TopLeft, p_pos},
        {ECorner::BotRight, p_pos},
        {ECorner::BotLeft, p_pos}
    };

    color = p_color;
}

const void Cell::Draw3D(Vector3 p_offset)
{
    Vector3 pos = Vector3Add(properties->centerPos, p_offset); 

    DrawCube(pos, 1.0f, 1.0f, 1.0f, WHITE);
}

const void Cell::Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight)
{
    int posX = properties->centerPos.x + p_offset.x;
    int posY = properties->centerPos.y + p_offset.y;

    DrawRectangle(posX * p_cellWidth, posY * p_cellHeight, p_cellWidth, p_cellHeight, color);
}

void Cell::SetCornersPos(std::map<ECorner, Vector3> p_cornerPos)
{
    for (const auto& [corner, pos] : p_cornerPos)
    {
        SetCornerPos(corner, pos);
    }
}

void Cell::SetCornerPos(ECorner p_corner, Vector3 p_pos)
{
    properties->cornerPos[p_corner] = p_pos;
}