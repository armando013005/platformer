#pragma once
#include "raylib.h"
#include "include/game.h"

class Entidad
{
public:
    Vector2 Position = { 20,160 };
    int texture = 300;
    Rectangle hitbox = {Position.x,Position.y,14,13};
    Color color = RED;
    Ray ray = {

    };
    bool canJump = false;
    float Vel_x = 0;
    float Vel_y = 1.f;


private:

};

typedef struct {
    Rectangle Rec;
    int Es; // 0 piso, 1 pared, 2 techo
    //int objetos = 4;
}PlatformHitbox;




void UpdatePlayer(Entidad* Player, PlatformHitbox* Objeto, int objetos);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);