#include <raylib.h>

#include "include/main.h"
#include "include/game.h"
#include "include/screens.h"
#include "include/loading.h"
Entidad Player;

const int objetos = 4;

PlatformHitbox Objeto[objetos]{
    {{0,224,240,16},0},
    {{16,16,1,224},1},
    {{240,16,1,224},1},
    {{16,30,224,1},2}
};


                        //solo un int
// {x,y,width,heigth},(0 piso, 1 pared, 2 techo)}
    
Camera2D camera = {
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f },
    camera.target = Player.Position,
    camera.rotation = 0,
    camera.zoom = 2.5f,

};


float velocidad = 0;
float VVertical = 1.f;
float PosX = 50;


void MovePlayer() {

    float Gravedad = 300;
    float deltatime = GetFrameTime();
    float aceleracion = 0.f;
    if (IsKeyDown(KEY_RIGHT)) aceleracion += 2600.f;
    if (IsKeyDown(KEY_LEFT)) aceleracion -= 2600.f;
    if (IsKeyDown(KEY_X) && Player.canJump) {
        VVertical -= 200.f;
        Player.canJump = false;
    }

    //if (IsKeyDown(KEY_DOWN)) Player.Position.y += 1;
    aceleracion -= velocidad * 25.f;
    
    Player.Position.x = Player.Position.x + velocidad * deltatime + aceleracion * deltatime * deltatime * .5f;
    velocidad = velocidad + aceleracion * deltatime;

    //update hitbox
    Player.hitbox = { Player.Position.x + 1,Player.Position.y + 4,14,12 };

    //jump
    bool ColPiso = false;
    bool ColPared = false;
    bool ColTecho = false;

    //piso
    for (int i = 0; i < objetos; i++)
    {
        PlatformHitbox* Pi = Objeto + i;
        Entidad* Pl = &Player;
        switch (Pi->Es)
        {
        case 0:
            if ( Pl->Position.x < Pi->Rec.x + Pi->Rec.width &&
                Pl->Position.x + Pl->hitbox.width > Pi->Rec.x &&
                Pl->Position.y < Pi->Rec.y + Pi->Rec.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Rec.y)
            {
                VVertical = 0;
                Pl->Position.y = Pi->Rec.y - 16;
                ColPiso = true;
            }
            break;
        case 1:
            if (Pl->Position.x < Pi->Rec.x + Pi->Rec.width &&
                Pl->Position.x + Pl->hitbox.width > Pi->Rec.x &&
                Pl->Position.y < Pi->Rec.y + Pi->Rec.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Rec.y)
            {
                velocidad *=  -1.5;
                ColPared = true;
            }
            break;
        case 2:
            break;
        }
        
    }
    

    if(!ColPiso) {
        Player.Position.y += VVertical*deltatime;
        VVertical += Gravedad * deltatime;
        Player.canJump = false;
        
    }
    else {
        Player.canJump = true;
    }
    
    

}

void DrawPlayer() {


    DrawTexture(GetTexture(0), Player.Position.x, Player.Position.y, WHITE);

    DrawText(TextFormat("px: %f",Player.Position.x), Player.Position.x, Player.Position.y - 40, 10, WHITE);

    DrawText(TextFormat("py: %f", Player.Position.y), Player.Position.x, Player.Position.y - 50, 10, WHITE);

    DrawText(TextFormat("v: %f", velocidad), Player.Position.x, Player.Position.y - 70, 10, WHITE);

    DrawText(TextFormat("vv: %f", VVertical), Player.Position.x, Player.Position.y - 60, 10, WHITE);

    //oDrawRectangleLinesEx(Player.hitbox, 1, GREEN);
}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        BeginMode2D(camera);
        
        DrawTexture(GetBackground(0), 0, 0, WHITE);
        
        DrawPlayer();
        
        /*for (int i = 0; i < 4; i++) {
            DrawRectangleLinesEx(Objeto[i].Rec, 1, GREEN);
        }*/
        
       
        EndMode2D();
        
    }
}game;


void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height)
{
    static Vector2 bbox = { 1.f, 1.f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, * camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, * camera);
    camera->offset = { ( 1-bbox.x) * 0.5f * width, (1- bbox.y) * 0.5f * height };

    if (player->Position.x < bboxWorldMin.x) camera->target.x = player->Position.x;
    if (player->Position.y < bboxWorldMin.y) camera->target.y = player->Position.y;
    if (player->Position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->Position.x - bboxWorldMax.x);
    if (player->Position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->Position.y - bboxWorldMax.y);
}

void UpdateGame() {
    
    
    MovePlayer();
    
    UpdateCameraPlayerBoundsPush(&camera,&Player,GetScreenWidth(),GetScreenHeight());


    SetActiveScreen(&game);
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}
