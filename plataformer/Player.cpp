#include "include/Player.h"
#include "include/game.h"
#include "include/map.h"
#include "include/tile_map.h"
#include <vector>
void UpdatePlayer(Entidad* Player) {


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
    for (const TileObject* Pi: GetMapObjectsOfType("Colider"))
    {
        Entidad* Pl = Player;
        std::string name = Pi->Name;
        
        enum class TiposDeColisiones
        {
            Pared,
            Piso,
            Picos,
            OneSide
        };

        TiposDeColisiones Tipo = TiposDeColisiones::Pared;
        if (name == "pared") {
            Tipo = TiposDeColisiones::Pared;
        }
        else if(name == "piso") {
            Tipo = TiposDeColisiones::Piso;
        }
        else if (name == "pico") {
            Tipo = TiposDeColisiones::Picos;
        }
        else if (name == "OneSide") {
            Tipo = TiposDeColisiones::OneSide;
        }

        switch (Tipo)
        {
        case TiposDeColisiones::Piso:
            if (Pl->hitbox.x < Pi->Bounds.x + Pi->Bounds.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Bounds.x &&
                Pl->hitbox.y < Pi->Bounds.y + Pi->Bounds.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Bounds.y)
            {
                //saltando
                if (Pl->Vel_y < 0) {
                    Pl->hitbox.y = Pi->Bounds.y + Pi->Bounds.height;
                    Pl->Vel_y = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }
                else//cayendo
                    if (Pl->Vel_y > 0) {
                        Pl->hitbox.y = Pi->Bounds.y - Pl->hitbox.height;
                        Pl->Vel_y = 0;
                        ColPiso = true;
                        //Pl->Position.x = Pl->hitbox.x;
                    }

                
            }
            break;
        case TiposDeColisiones::Pared:
            if (Pl->hitbox.x < Pi->Bounds.x + Pi->Bounds.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Bounds.x &&
                Pl->hitbox.y < Pi->Bounds.y + Pi->Bounds.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Bounds.y
                )
            {

                if (Pl->Vel_x > 0) {
                    Pl->hitbox.x = Pi->Bounds.x - Pl->hitbox.width;
                    Pl->Vel_x = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }else
                if (Pl->Vel_x < 0) {
                    Pl->hitbox.x = Pi->Bounds.x + Pi->Bounds.width;
                    Pl->Vel_x = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }
            }
            break;
        case TiposDeColisiones::OneSide:
            if (Pl->hitbox.x < Pi->Bounds.x + Pi->Bounds.width &&
                Pl->hitbox.x + Pl->hitbox.width > Pi->Bounds.x &&
                Pl->hitbox.y < Pi->Bounds.y + Pi->Bounds.height &&
                Pl->hitbox.height + Pl->hitbox.y > Pi->Bounds.y)
            {
                
                if (Pl->Vel_y < 0) {
                    //Pl->hitbox.y = Pi->Bounds.y + Pi->Bounds.height;
                    //Pl->Vel_y = 0;
                    //Pl->Position.x = Pl->hitbox.x;
                }
                else//cayendo
                    if (Pl->Vel_y > 0) {
                        Pl->hitbox.y = Pi->Bounds.y - Pl->hitbox.height;
                        Pl->Vel_y = 0;
                        ColPiso = true;
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
 
    static Vector2 bbox = { 0.5f, .6f };

    Vector2 bboxWorldMin = GetScreenToWorld2D({ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D({ (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, *camera);
    camera->offset = { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

    if (player->hitbox.x < bboxWorldMin.x) camera->target.x = player->Position.x;
    if (player->hitbox.y < bboxWorldMin.y) camera->target.y = 16;
    if (player->hitbox.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->Position.x - bboxWorldMax.x);
    if (player->hitbox.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->Position.y - bboxWorldMax.y);
}