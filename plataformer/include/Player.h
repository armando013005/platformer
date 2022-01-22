#pragma once
#include "raylib.h"
#include "include/game.h"
#include "map.h"
#include "ferox.h"

class Entidad
{
public:
    int texture = 300;
    Color color = RED;
    frBody* Body;
  

    bool firstinit = true;
    bool canJump = false;
    float Vel_x = 0;
    float Vel_y = 1.f;
    int maxP;


private:

};






void UpdatePlayer(Entidad* Player, frWorld* world);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);