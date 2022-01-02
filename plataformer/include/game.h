#pragma once
#include <raylib.h>



class Entidad
{
public:
    Vector2 Position = { 20,70 };
    Rectangle hitbox = { Position.x + 1,Position.y + 4,14,12 };
    Color color = RED;
    Ray ray = {

    };
    bool canJump = false;
    
    

private:

};

typedef struct {
    Rectangle Rec;
    int Es; // 0 piso, 1 pared, 2 techo

}PlatformHitbox;

void MovePlayer();

void DrawPlayer();

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);

void UpdateGame();


            
 