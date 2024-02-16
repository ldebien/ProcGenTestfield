#pragma once

#include <map>

#include "raylib.h"

enum ECorner
{
    TopRight    = 0,
    TopLeft     = 1,
    BotLeft     = 2,
    BotRight    = 3
};

struct CellProperties
{
    bool isFloor {false};
    Vector3 centerPos {};
    std::map<ECorner, Vector3> cornerPos {};
};

class Cell
{
public:
    Cell();
    ~Cell();

    const Vector3 GetPos()      { return properties->centerPos; }
    const Vector2 GetPos2D()    { return Vector2 { properties->centerPos.x, properties->centerPos.y }; }
    const bool IsFloor()        { return properties->isFloor; }

    void SetFloorState(bool p_state) { properties->isFloor = p_state; }

    void Init(Vector3 p_pos, bool p_floorState, Color p_color);

    const void Draw3D(Vector3 p_offset);
    const void Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight);

    void SetCornersPos(std::map<ECorner, Vector3> p_cornerPos);

private:
    CellProperties *properties {};
    Color color {};

    void SetCornerPos(ECorner p_corner, Vector3 p_pos);
};