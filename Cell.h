#pragma once

#include <map>
#include <vector>

#include "raylib.h"

enum EDirection
{
    Right       = 0,
    TopRight    = 1,
    Top         = 2,
    TopLeft     = 3,
    Left        = 4,
    BotLeft     = 5,
    Bot         = 6,
    BotRight    = 7,
    Center      = 8
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
    std::map<EDirection, Vector3> cornerPos {};
};

class Cell
{
public:
    Cell();
    ~Cell();

    const Vector3 GetPos()                                      { return properties->centerPos; }
    const Vector2 GetPos2D()                                    { return Vector2 { properties->centerPos.x, properties->centerPos.z }; }

    const ECellType GetCellType()                               { return properties->type; }
    void SetFloorType()                                         { SetType(ECellType::Floor); }
    void SetWallType()                                          { SetType(ECellType::Wall); }
    void ResetType()                                            { SetType(ECellType::Empty); }
    const Color GetColor(Color p_newColor)                      { return color; }
    void SetColor(Color p_newColor)                             { color = p_newColor; }
    void SetNeighbour(EDirection p_neighbourDir, Cell* p_cell)  { neighbours[p_neighbourDir] = p_cell; }

    void Init(Vector3 p_pos);
    void SetCornersPosY(std::map<EDirection, float> p_cornerPos);
    const std::vector<int> GetVertexIndex(EDirection p_vertex); 

    const void Draw3D(Vector3 p_offset);
    const void Draw2D(Vector2 p_offset, int p_cellWidth, int p_cellHeight);

private:
    CellProperties *properties {};
    std::map<EDirection, Cell*> neighbours {};
    Mesh mesh {0};
    Model model {0};
    Mesh *cellMesh {};
    Color color {};

    void SetType(ECellType p_type);
    void SetCornerPosY(EDirection p_corner, float p_posY);
    void GenerateMesh();
    void GenerateFloorMesh();
    void UpdateMesh();
    void UpdateFloorMeshHeight();
    
};