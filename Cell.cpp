#include "Cell.h"

#include "raymath.h"

Cell::Cell()
{
    properties = new CellProperties
    {
        ECellType::Empty,
        Vector3Zero(),
        {
            {ECellVertice::Right,       Vector3Zero()},
            {ECellVertice::TopRight,    Vector3Zero()},
            {ECellVertice::Top,         Vector3Zero()},
            {ECellVertice::TopLeft,     Vector3Zero()},
            {ECellVertice::Left,        Vector3Zero()},
            {ECellVertice::BotLeft,     Vector3Zero()},
            {ECellVertice::Bot,         Vector3Zero()},
            {ECellVertice::BotRight,    Vector3Zero()}
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
    properties->cornerPos =
    {
        {ECellVertice::Right,       Vector3Add(p_pos, {0.5f, 0.0f, 0.0f})},
        {ECellVertice::TopRight,    Vector3Add(p_pos, {0.5f, 0.0f, -0.5f})},
        {ECellVertice::Top,         Vector3Add(p_pos, {0.0f, 0.0f, -0.5f})},
        {ECellVertice::TopLeft,     Vector3Add(p_pos, {-0.5f, 0.0f, -0.5f})},
        {ECellVertice::Left,        Vector3Add(p_pos, {-0.5f, 0.0f, 0.0f})},
        {ECellVertice::BotLeft,     Vector3Add(p_pos, {-0.5f, 0.0f, 0.5f})},
        {ECellVertice::Bot,         Vector3Add(p_pos, {0.0f, 0.0f, 0.5f})},
        {ECellVertice::BotRight,    Vector3Add(p_pos, {0.5f, 0.0f, 0.5f})}
    };

    color = BLANK;
}

void Cell::SetCornersPosY(std::map<ECellVertice, float> p_cornerPosY)
{
    for (const auto& [corner, posY] : p_cornerPosY)
    {
        SetCornerPosY(corner, posY);
    }
}

void Cell::SetCornerPosY(ECellVertice p_corner, float p_posY)
{
    properties->cornerPos[p_corner].y = p_posY;
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

    if (properties->type == ECellType::Wall)
    {
        return;
    }

    float correctedY = 0;
    for (auto const& [corner, pos] : properties->cornerPos)
    {
        correctedY += pos.y;
    }

    Vector3 correctedCenter = properties->centerPos;
    correctedCenter.y = correctedY / properties->cornerPos.size();

    Vector3 *vertices = new Vector3[12] {};

    //Top Left
    *vertices = Vector3Add(properties->cornerPos[ECellVertice::TopLeft], p_offset);
    // Left
    *(vertices+1) = Vector3Add(properties->cornerPos[ECellVertice::Left], p_offset);
    // Top
    *(vertices+2) = Vector3Add(properties->cornerPos[ECellVertice::Top], p_offset);
    // Center
    *(vertices+3) = Vector3Add(correctedCenter, p_offset);
    // Top Right
    *(vertices+4) = Vector3Add(properties->cornerPos[ECellVertice::TopRight], p_offset);
    // Right
    *(vertices+5) = Vector3Add(properties->cornerPos[ECellVertice::Right], p_offset);
    // Left
    *(vertices+6) = Vector3Add(properties->cornerPos[ECellVertice::Left], p_offset);
    // Bot Left
    *(vertices+7) = Vector3Add(properties->cornerPos[ECellVertice::BotLeft], p_offset);
    // Center
    *(vertices+8) = Vector3Add(correctedCenter, p_offset);
    // Bot
    *(vertices+9) = Vector3Add(properties->cornerPos[ECellVertice::Bot], p_offset);
    // Right
    *(vertices+10) = Vector3Add(properties->cornerPos[ECellVertice::Right], p_offset);
    // Bot Right
    *(vertices+11) = Vector3Add(properties->cornerPos[ECellVertice::BotRight], p_offset);

    DrawTriangleStrip3D(vertices, 12, color);
}

const void Cell::Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight)
{
    Vector2 centerPos = GetPos2D();
    int posX = centerPos.x + p_offset.x;
    int posY = centerPos.y + p_offset.y;

    DrawRectangle(posX * p_cellWidth, posY * p_cellHeight, p_cellWidth, p_cellHeight, color);
}