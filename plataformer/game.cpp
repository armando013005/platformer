#include <raylib.h>

#include "include/main.h"
#include "include/game.h"
#include "include/screens.h"
#include "include/loading.h"
Entidad Player;
Plataforma piso;

PlatformHitbox hitboxes[4] = {
    {{16,16,224,15},0,0,0 },//arriba
    {{0,16,16,208},0,1,0 },//izquerda
    {{16,224,224,16},0,0,1 },//abajo
    {{240,16,15,224},0,1,0 },//derecha
};

// {x,y,width,heigth},pico, pared,piso}
    
Camera2D camera = {
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f },
    camera.target = Player.Position,
    camera.rotation = 0,
    camera.zoom = 1.f,

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
    bool collision = false;
    for (int i = 0; i < 4; i++)
    {
        PlatformHitbox* ei = hitboxes + i;
        Vector2* p = &(Player.Position);
        if (ei->piso &&
            ei->Rec.x <= p->x &&
            ei->Rec.x + ei->Rec.width >= p->x &&
            ei->Rec.y-16 >= p->y &&
            ei->Rec.y-16 < p->y + VVertical * deltatime)
        {
            collision = 1;
            VVertical= 0.0f;
            p->y = ei->Rec.y-16;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        PlatformHitbox* ei = hitboxes + i;
        Vector2* p = &(Player.Position);
        if (ei->pared &&
            ei->Rec.x <= p->x &&
            ei->Rec.x + ei->Rec.width <= p->x  &&
            ei->Rec.y >= p->y &&
            ei->Rec.y + ei->Rec.height >= p->y )
        {
            //collision = 1;
            velocidad = 0.0f;
            p->x = ei->Rec.x;
        }
    }
    
    if(!collision) {
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
    DrawText(TextFormat("px: %f",Player.Position.x, "  py: %f",Player.Position.y), Player.Position.x, Player.Position.y - 50, 10, WHITE);
    DrawText(TextFormat("v: %f", velocidad), Player.Position.x, Player.Position.y - 70, 10, WHITE);
    DrawText(TextFormat("vv: %f", VVertical), Player.Position.x, Player.Position.y - 60, 10, WHITE);

    DrawRectangleLinesEx(Player.hitbox, 1, GREEN);
}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        BeginMode2D(camera);
        
        DrawTexture(GetBackground(0), 0, 0, WHITE);
        
        DrawPlayer();
        
        for (int i = 0; i < 4; i++) {
            DrawRectangleLinesEx(hitboxes[i].Rec,1,GREEN);
        }
        
       // DrawRectangleLinesEx(hitboxes[2].Rec, 1, GREEN);
        EndMode2D();
        
    }
}game;


void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height)
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, * camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, * camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

    if (player->Position.x < bboxWorldMin.x) camera->target.x = player->Position.x;
    if (player->Position.y < bboxWorldMin.y) camera->target.y = player->Position.y;
    if (player->Position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->Position.x - bboxWorldMax.x);
    if (player->Position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->Position.y - bboxWorldMax.y);
}

void UpdateGame() {
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
    
    MovePlayer();
    
    UpdateCameraPlayerBoundsPush(&camera,&Player,GetScreenWidth(),GetScreenHeight());


    SetActiveScreen(&game);
}
