#pragma once
#include "raylib.h"
#include "include/game.h"

class Entidad
{
public:
    Vector2 Position = { 20,70 };
    int texture = 300;
    Rectangle hitbox = {0,0,0,0};
    Color color = RED;
    Ray ray = {

    };
    bool canJump = false;
    float velocidad = 0;
    float VVertical = 1.f;
    float PosX = 50;


private:

};

typedef struct {
    Rectangle Rec;
    int Es; // 0 piso, 1 pared, 2 techo
    //int objetos = 4;
}PlatformHitbox;


void MovePlayer(Entidad* Player);

void Collisions(Entidad* Player, PlatformHitbox* Objeto, int objetos);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);