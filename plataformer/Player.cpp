#include "Player.h"
#include "include/game.h"

void UpdatePlayer(Entidad* Player,PlatformHitbox* Objeto, int objetos) {


    float deltatime = GetFrameTime();
    float aceleracion = 0.f;
    float Gravedad = 300;
    bool ColPiso = false;

    float prePosX = Player->hitbox.x;
    float prePosY = Player->hitbox.y;

    if (IsKeyDown(KEY_RIGHT)) aceleracion += 2000.f;
    if (IsKeyDown(KEY_LEFT)) aceleracion -= 2000.f;
    if (IsKeyDown(KEY_X) && Player->canJump) {
        Player->canJump = false;
        Player->Vel_y -= 200.f;
        
    }

    
    aceleracion -= Player->Vel_x * 10.f;

    Player->hitbox.x = Player->Position.x + Player->Vel_x * deltatime + aceleracion * deltatime * deltatime * .5f;
    Player->Vel_x = Player->Vel_x + aceleracion * deltatime;

    Player->hitbox.y += Player->Vel_y * deltatime;
    Player->Vel_y += Gravedad * deltatime;


    //collisions

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
                Pl->hitbox.height + Pl->hitbox.y > Pi->Rec.y)
            {
                //saltando
                if (Pl->Vel_y < 0) {
                    Pl->hitbox.y = Pi->Rec.y + Pi->Rec.height;
                    Pl->Vel_y = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }
                else//cayendo
                    if (Pl->Vel_y > 0) {
                        Pl->hitbox.y = Pi->Rec.y - Pl->hitbox.height;
                        Pl->Vel_y = 0;
                        ColPiso = true;
                        //Pl->Position.x = Pl->hitbox.x;
                    }

                
            }
            break;
        case 1:
            if (Pl->hitbox.x < Pi->Rec.x + Pi->Rec.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Rec.x &&
                Pl->hitbox.y < Pi->Rec.y + Pi->Rec.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Rec.y
                )
            {

                if (Pl->Vel_x > 0) {
                    Pl->hitbox.x = Pi->Rec.x - Pl->hitbox.width;
                    Pl->Vel_x = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }else
                if (Pl->Vel_x < 0) {
                    Pl->hitbox.x = Pi->Rec.x + Pi->Rec.width;
                    Pl->Vel_x = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }
            }
            break;
        }

    }


    if (!ColPiso) {
        Player->canJump = false;

    }
    else {
        Player->canJump = true;
    }

    Player->Position.y = Player->hitbox.y;
    Player->Position.x = Player->hitbox.x;
}

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height)
{
    static Vector2 bbox = { 0.6f, 0.6f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

    if (player->hitbox.x < bboxWorldMin.x) camera->target.x = player->Position.x;
    if (player->hitbox.y < bboxWorldMin.y) camera->target.y = player->Position.y;
    if (player->hitbox.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->Position.x - bboxWorldMax.x);
    if (player->hitbox.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->Position.y - bboxWorldMax.y);
}