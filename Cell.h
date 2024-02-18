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

enum ECellType
{
    Empty   = 0,
    Floor   = 1,
    Wall    = 2
};

struct CellProperties
{
    ECellType type { ECellType::Empty };
    Vector3 centerPos {};
    std::map<ECorner, Vector3> cornerPos {};
};

class Cell
{
public:
    Cell();
    ~Cell();

    const Vector3 GetPos()                                  { return properties->centerPos; }
    const Vector2 GetPos2D()                                { return Vector2 { properties->centerPos.x, properties->centerPos.z }; }

    const ECellType GetCellType()                           { return properties->type; }
    void SetFloorType()                                     { SetType(ECellType::Floor); }
    void SetWallType()                                      { SetType(ECellType::Wall); }
    void ResetType()                                        { SetType(ECellType::Empty); }
    const Color GetColor(Color p_newColor)                  { return color; }
    void SetColor(Color p_newColor)                         { color = p_newColor; }

    void Init(Vector3 p_pos);
    void SetCornersPos(std::map<ECorner, Vector3> p_cornerPos);

    const void Draw3D(Vector3 p_offset);
    const void Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight);

private:
    CellProperties *properties {};
    Color color {};

    void SetType(ECellType p_type)                           { properties->type = p_type; }
    void SetCornerPos(ECorner p_corner, Vector3 p_pos);
};