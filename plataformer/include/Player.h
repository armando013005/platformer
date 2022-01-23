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
  
    bool canJump = false;
  
    bool WeDashed = false;
    bool WantToMoveL = false;

    int maxP;

    float GroundedRememberTime = 0.065;
    float GroundedRemember = 0;

    float RememberJumpTime = 0.02;
    float RememberJump = 0;


private:

};






void UpdatePlayer(Entidad* Player, frWorld* world);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);