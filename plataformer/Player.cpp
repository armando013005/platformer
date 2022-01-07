#include "Player.h"
#include "include/game.h"

void MovePlayer(Entidad* Player) {


    float deltatime = GetFrameTime();
    float aceleracion = 0.f;
    if (IsKeyDown(KEY_RIGHT)) aceleracion += 2000.f;
    if (IsKeyDown(KEY_LEFT)) aceleracion -= 2000.f;
    if (IsKeyDown(KEY_X) && Player->canJump) {
        Player->VVertical -= 200.f;
        Player->canJump = false;
    }

    //if (IsKeyDown(KEY_DOWN)) Player->Position.y += 1;
    aceleracion -= Player->velocidad * 10.f;

    Player->Position.x = Player->Position.x + Player->velocidad * deltatime + aceleracion * deltatime * deltatime * .5f;
    Player->velocidad = Player->velocidad + aceleracion * deltatime;

    //update hitbox
    Player->hitbox = { Player->Position.x ,Player->Position.y,14,13 };
}


void Collisions(Entidad* Player, PlatformHitbox* Objeto, int objetos) {
    //jump
    bool ColPiso = false;
    bool ColPared = false;
    bool ColTecho = false;

    float deltatime = GetFrameTime();
    float Gravedad = 300;
    Rectangle DummyRecX = { Player->hitbox.x + Player->velocidad*deltatime, Player->hitbox.y, Player->hitbox.width,Player->hitbox.height };
    //piso
    for (int i = 0; i < objetos; i++)
    {
        PlatformHitbox* Pi = Objeto + i;
        Entidad* Pl = Player;
        switch (Pi->Es)
        {
        case 0:
            if (Pl->hitbox.x < Pi->Rec.x + Pi->Rec.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Rec.x &&
                Pl->hitbox.y < Pi->Rec.y + Pi->Rec.height &&
                Pl->hitbox.height + Pl->hitbox.y + Pl->VVertical*deltatime > Pi->Rec.y)
            {
                Pl->VVertical = 0;
                Pl->Position.y = Pi->Rec.y - Pl->hitbox.height;
                
                ColPiso = true;
            }
            break;
        case 1:
            if (Pl->hitbox.x < Pi->Rec.x + Pi->Rec.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Rec.x &&
                Pl->hitbox.y < Pi->Rec.y + Pi->Rec.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Rec.y
                )
            {
                
                if (Pl->velocidad > 0) {
                    Pl->Position.x = Pi->Rec.x - Pl->hitbox.width;
                    //Pl->Position.x = Pl->hitbox.x;
                }
                if (Pl->velocidad < 0) {
                    Pl->Position.x = Pi->Rec.x + Pi->Rec.width;
                    //Pl->Position.x = Pl->hitbox.x;
                }
            }
            break;
        }

    }


    if (!ColPiso) {
        Player->Position.y += Player->VVertical * deltatime;
        Player->VVertical += Gravedad * deltatime;
        Player->canJump = false;

    }
    else {
        Player->canJump = true;
    }

}

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height)
{
    static Vector2 bbox = { 0.6f, 0.6f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

    if (player->Position.x < bboxWorldMin.x) camera->target.x = player->Position.x;
    if (player->Position.y < bboxWorldMin.y) camera->target.y = player->Position.y;
    if (player->Position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->Position.x - bboxWorldMax.x);
    if (player->Position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->Position.y - bboxWorldMax.y);
}