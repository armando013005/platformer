#include <raylib.h>

#include "include/game.h"
#include "include/Player.h"
#include "include/main.h"
#include "include/screens.h"
#include "include/loading.h"
#include "include/sprites.h"
#include "include/map.h"
#include "include/tile_map.h"

Entidad Player;

                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical,2 playaforma flotante)}
    
Camera2D camera = {
    camera.offset = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f },
    camera.target = Player.Position,
    camera.rotation = 0,
    camera.zoom = 2.5f,

};



void DrawPlayer() {


    DrawSprite(Player.texture, Player.Position.x, Player.Position.y,0,1.f, WHITE, SpriteFlipNone);

    //DrawRectangleLinesEx(Player.hitbox,0.5,GREEN);
    /*
    DrawText(TextFormat("Pos X: %f",Player.Position.x), Player.Position.x, Player.Position.y - 40, 10, WHITE);

    DrawText(TextFormat("Pos Y: %f", Player.Position.y), Player.Position.x, Player.Position.y - 50, 10, WHITE);

    DrawText(TextFormat("Vel X: %f", Player.Vel_x), Player.Position.x, Player.Position.y - 70, 10, WHITE);

    DrawText(TextFormat("Vel Y: %f", Player.Vel_y), Player.Position.x, Player.Position.y - 60, 10, WHITE);
*/
    //oDrawRectangleLinesEx(Player.hitbox, 1, GREEN);
}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        DrawMap();
        BeginMode2D(GetMapCamera());
        
        
        DrawPlayer();
        
        /*for (const TileObject* object : GetMapObjectsOfType("Colider")) {
            DrawRectangleLinesEx(object->Bounds,0.5f, GREEN);
        }*/
       
        EndMode2D();
        
    }
}game;




void UpdateGame() {
    
    
    UpdatePlayer(&Player);
    
    UpdateCameraPlayerBoundsPush(&GetMapCamera(),&Player,GetScreenWidth(),GetScreenHeight());


    SetActiveScreen(&game);
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}
