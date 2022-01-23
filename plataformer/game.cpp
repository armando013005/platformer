#include <raylib.h>

#include "include/game.h"
#include "include/Player.h"
#include "include/main.h"
#include "include/screens.h"
#include "include/loading.h"
#include "include/sprites.h"
#include "include/map.h"
#include "include/tile_map.h"
#include "ferox.h"
#include <vector>


#define PLAYER_MATERIAL   { 23.f, 0.0f, 0.0f, 0.9f }

bool firstLoad = true;
int currentlvl;
std::vector<const char*> niveles;
                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical,2 playaforma flotante)}

Entidad Player;
    
juego Juego;


void NextLevel() {
    currentlvl++;
    if (niveles.empty() || currentlvl > niveles.size() )
       return;
    LoadMap(niveles[currentlvl]);
    initphysics();
}

void PushLevels() {
    niveles.push_back("nullptr");
    niveles.push_back("resources/levels/defaulttest.tmx");

    NextLevel();
}

void initphysics()
{
    const TileObject* world = GetFirstMapObjectOfType("World");
    
    if (Player.maxP > 0) {
        frClearWorld(Juego.f_world);
        Player.maxP = 0;
        frSetWorldBounds(Juego.f_world,frRecPixelsToMeters(world->Bounds));
    }
    else {
        Juego.f_world = frCreateWorld(frVec2ScalarMultiply({ 0.f,15.0f }, 0.00001f), frRecPixelsToMeters(world->Bounds));
    }
    
    const TileObject* Spawn = GetFirstMapObjectOfType("Spawn");
    std::vector <const TileObject*> walls = GetMapObjectsOfType("Colider");

    

    Player.Body = frCreateBodyFromShape(
        FR_BODY_DYNAMIC,
        FR_FLAG_INFINITE_INERTIA,
        frVec2PixelsToMeters({ Spawn->Bounds.x + 14/2,Spawn->Bounds.y+13/2 }),
        frCreateRectangle(PLAYER_MATERIAL, frNumberPixelsToMeters(12),
            frNumberPixelsToMeters(10))
    );
    
    frAddToWorld(Juego.f_world, Player.Body);

    for (auto m : walls) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
        FR_BODY_STATIC,
        FR_FLAG_WALL,
        frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width/2), m->Bounds.y + (m->Bounds.height/2) }),
        frCreateRectangle({ 1.25f, 0.0f, 0.7f, 0.7f },frNumberPixelsToMeters(m->Bounds.width),frNumberPixelsToMeters(m->Bounds.height))
        );

        frAddToWorld(Juego.f_world,wal);
    }

}

void UpdatePhysics()
{
    frSetBodyRotation(Player.Body, 0);
    frSetBodyAngularVelocity(Player.Body, 0);
    frSimulateWorld(Juego.f_world,GetFrameTime() * 100);
    frSetBodyAngularVelocity(Player.Body, 0);
    frSetBodyRotation(Player.Body, 0);
}

void DrawPlayer() {

    Vector2 pos = frVec2MetersToPixels(frGetBodyPosition(Player.Body));
    DrawSprite(300, pos.x-7, pos.y-8, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);

}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        DrawMap();
        BeginMode2D(GetMapCamera());
        
        
        /*for (int i = 0; i < frGetWorldBodyCount(Juego.f_world); i++) {
            frBody* body = frGetWorldBody(Juego.f_world, i);

            frDrawBodyAABB(body,GREEN);
        }*/
        DrawPlayer();
        //frDrawBody(Player.Body,GREEN);

        if (Player.WantToMoveL) {
            DrawText("want to move",150,-60,20,WHITE);
        }
       
        Vector2 vel = frGetBodyVelocity(Player.Body);
        DrawText(TextFormat("velocityx %f ",vel.x), 0, -30, 20, WHITE);
        DrawText(TextFormat("velocityy %f ", vel.y),300, -30, 20, WHITE);

        DrawFPS(-5,-50);
        EndMode2D();
        
    }
}game;




void UpdateGame() {
    
    if (firstLoad) {
        PushLevels();

        firstLoad = false;
    }

   UpdatePlayer(&Player,Juego.f_world);
    
    //UpdateCameraPlayerBoundsPush(&GetMapCamera(),&Player,800,600);

    
    
    SetActiveScreen(&game);

    UpdatePhysics();
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}

