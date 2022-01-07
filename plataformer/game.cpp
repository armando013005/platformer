#include <raylib.h>

#include "include/game.h"
#include "Player.h"
#include "include/main.h"
#include "include/screens.h"
#include "include/loading.h"
#include "include/sprites.h"

Entidad Player;

const int objetos = 4;

PlatformHitbox Objeto[objetos]{
    {{0,224,240,16},0},
    {{0,16,16,224},1},
    {{240,16,16,224},1},
    {{16,18,224,16},0}
};


                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical)}
    
Camera2D camera = {
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f },
    camera.target = Player.Position,
    camera.rotation = 0,
    camera.zoom = 2.5f,

};



void DrawPlayer() {


    DrawSprite(Player.texture, Player.Position.x, Player.Position.y,0,1.f, WHITE, SpriteFlipNone);

    DrawRectangleLinesEx(Player.hitbox,0.5,GREEN);

    DrawText(TextFormat("px: %f",Player.Position.x), Player.Position.x, Player.Position.y - 40, 10, WHITE);

    DrawText(TextFormat("py: %f", Player.Position.y), Player.Position.x, Player.Position.y - 50, 10, WHITE);

    DrawText(TextFormat("v: %f", Player.Vel_x), Player.Position.x, Player.Position.y - 70, 10, WHITE);

    DrawText(TextFormat("vv: %f", Player.Vel_y), Player.Position.x, Player.Position.y - 60, 10, WHITE);

    //oDrawRectangleLinesEx(Player.hitbox, 1, GREEN);
}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        BeginMode2D(camera);
        
        DrawTexture(GetTexture(2), 0, 0, WHITE);
        
        DrawPlayer();
        
        for (int i = 0; i < 4; i++) {
            DrawRectangleLinesEx(Objeto[i].Rec, 0.5, GREEN);
        }
        
       
        EndMode2D();
        
    }
}game;




void UpdateGame() {
    
    
    UpdatePlayer(&Player, &Objeto[0], objetos);
    
    UpdateCameraPlayerBoundsPush(&camera,&Player,GetScreenWidth(),GetScreenHeight());


    SetActiveScreen(&game);
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}
