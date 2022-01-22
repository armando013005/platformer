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

#define PLAYER_MATERIAL   { 15.f, 0.0f, 1.0f, 1.f }

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
    const TileObject* Spawn = GetFirstMapObjectOfType("Spawn");
    std::vector <const TileObject*> walls = GetMapObjectsOfType("Colider");

    Juego.f_world = frCreateWorld(frVec2ScalarMultiply({ 0.f,15.0f }, 0.00001f), world->Bounds);

    Player.Body = frCreateBodyFromShape(
        FR_BODY_DYNAMIC,
        FR_FLAG_NONE,
        frVec2PixelsToMeters({ Spawn->Bounds.x + 14/2,Spawn->Bounds.y+13/2 }),
        frCreateRectangle(PLAYER_MATERIAL, frNumberPixelsToMeters(14),
            frNumberPixelsToMeters(13))
    );
    
    frAddToWorld(Juego.f_world, Player.Body);

    for (auto m : walls) {
        frBody* wal = frCreateBodyFromShape(
        FR_BODY_STATIC,
        FR_FLAG_WALL,
        frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width/2), m->Bounds.y + (m->Bounds.height/2) }),
        frCreateRectangle({ 1.25f, 0.0f, .85f, 0.6f },frNumberPixelsToMeters(m->Bounds.width),frNumberPixelsToMeters(m->Bounds.height))
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
    DrawSprite(300, pos.x-14/2, pos.y-13/2, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);

}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        DrawMap();
        BeginMode2D(GetMapCamera());
        
        DrawPlayer();
        
        /*for (int i = 0; i < frGetWorldBodyCount(Juego.f_world); i++) {
            frBody* body = frGetWorldBody(Juego.f_world, i);

            frDrawBodyAABB(body,GREEN);
        }*/
        //frDrawBody(Player.Body,GREEN);
       
        EndMode2D();
        
    }
}game;




void UpdateGame() {
    
    if (firstLoad) {
        PushLevels();

        firstLoad = false;
    }

   UpdatePlayer(&Player);
    
    //UpdateCameraPlayerBoundsPush(&GetMapCamera(),&Player,800,600);

    
    
    SetActiveScreen(&game);

    UpdatePhysics();
    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}

