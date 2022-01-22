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
    float Vel_x = 0;
    float Vel_y = 1.f;


private:

};

typedef struct {
    Rectangle Rec;
    int Es; // 0 piso, 1 pared, 2 techo
    //int objetos = 4;
}PlatformHitbox;




void UpdatePlayer(Entidad* Player);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);