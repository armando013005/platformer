#pragma once
#include <raylib.h>


class Plataforma {
public:
    Rectangle Plataformas = { 5,650,600,100 };
    Color color = GRAY;
    Texture2D textura;
    bool piso = true;

    //void Dibujar();
};

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
    bool pico = 0;
    bool pared = 0;
    bool piso = 0;
    Color color = GREEN;

}PlatformHitbox;

void MovePlayer();

void DrawPlayer();

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);

void UpdateGame();


            
 