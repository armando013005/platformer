#include <raylib.h>

#include "include/game.h"
#include "include/Player.h"
#include "include/main.h"
#include "include/screens.h"
#include "include/loading.h"
#include "include/sprites.h"
#include "include/map.h"
#include "include/tile_map.h"
#include <iostream>
#include "ferox.h"
#include <vector>
#include <memory>


#define PLAYER_MATERIAL   { 23.f, 0.0f, 0.0f, 0.f }

bool firstLoad = true;
int currentlvl;
std::vector<const char*> niveles;

                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical,2 playaforma flotante)}

Entidad Player;
    
juego Juego;

void NextLevel() {
    if ( currentlvl + 1 != niveles.size()) 
    {
        currentlvl++;
        if (niveles.empty() || 3 == currentlvl) { gotomenu(); }
      
        LoadMap(niveles[currentlvl]);

         Player.CanMove = Player.CanMoveRememberTime;
         StartTimer(&Player.timer,5.f);
        initphysics();
    }
}

void PushLevels() {
    niveles.push_back("nullptr");
    niveles.push_back("resources/levels/defaulttest.tmx");
    niveles.push_back("resources/levels/walljumpdemo.tmx");
    currentlvl = 0;

    NextLevel();
}

void initphysics()
{
    const TileObject* world = GetFirstMapObjectOfType("World");
    
    if (Player.maxP > 0) {
        frClearWorld(Juego.f_world);
        Player.maxP = 0;
        Player.CanMove = Player.CanMoveRememberTime;
        frSetWorldBounds(Juego.f_world,frRecPixelsToMeters(world->Bounds));
    }
    else {
        Juego.f_world = frCreateWorld(frVec2ScalarMultiply({ 0.f,15.0f }, 0.00001f), frRecPixelsToMeters(world->Bounds));
    }
    
    const TileObject* Spawn = GetFirstMapObjectOfType("Spawn");
    const TileObject* Extit = GetFirstMapObjectOfType("Exit");
    std::vector <const TileObject*> walls = GetMapObjectsOfType("Colider");
    std::vector<const TileObject*> picos = GetMapObjectsOfType("Pico", TileObject::SubTypes::Polygon);


    Player.Exit = Extit->Bounds;

    Player.Body = frCreateBodyFromShape(
        FR_BODY_DYNAMIC,
        FR_FLAG_INFINITE_INERTIA,
        frVec2PixelsToMeters({ Spawn->Bounds.x + 14/2,Spawn->Bounds.y+13/2 }),
        frCreateRectangle(PLAYER_MATERIAL, frNumberPixelsToMeters(10),
            frNumberPixelsToMeters(10))
    );


    frAddToWorld(Juego.f_world, Player.Body);

    for (auto m : picos) {
        Player.maxP++;
        
        const TilePolygonObject* poly = static_cast<const TilePolygonObject*>(m);

        frBody* pico = frCreateBodyFromShape(
            FR_BODY_STATIC,
            FR_FLAG_PICO,
            frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width / 2), m->Bounds.y + (m->Bounds.height / 2) }),
            frCreatePolygon({ 1.25f, 0.0f, 0.0f, 0.0f }, { { frVec2PixelsToMeters(poly->Points[0]),frVec2PixelsToMeters(poly->Points[1]),frVec2PixelsToMeters(poly->Points[2]),frVec2PixelsToMeters(poly->Points[3])},4})
        );

        frAddToWorld(Juego.f_world, pico);
    }
   
    for (auto m : walls) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
        FR_BODY_STATIC,
        FR_FLAG_WALL,
        frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width/2), m->Bounds.y + (m->Bounds.height/2) }),
        frCreateRectangle({ 1.25f, 0.0f, 0.0f, 0.0f },frNumberPixelsToMeters(m->Bounds.width),frNumberPixelsToMeters(m->Bounds.height))
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
    if (Player.WantToMoveL) {
        if (Player.WallSliding) {
            DrawSprite(546, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
        }
        else if(!(Player.GroundedRemember > 0)) {
            DrawSprite(544, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
        }
        else {
            DrawSprite(541, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
            DrawSprite(542, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
            DrawSprite(543, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
            DrawSprite(541, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
        }
    }
    else if(Player.WantToMoveR) {
        if(Player.WallSliding) {
            DrawSprite(546, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);
        }
        else if (!(Player.GroundedRemember > 0)) {
            DrawSprite(541, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);
        }
        else {
              DrawSprite(541, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);
              DrawSprite(542, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);
              DrawSprite(543, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);
              DrawSprite(541, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipNone);

        }
    }
    else {
        if (!(Player.GroundedRemember > 0)) {
            DrawSprite(545, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
        }
        else {
            DrawSprite(540, pos.x, pos.y, frGetBodyRotation(Player.Body), 1.f, WHITE, SpriteFlipX);
        }
        
    }

    

}
void debugmode() {
    DrawFPS(-5, -50);
    //std::cout << "CANMoVE: "<<Player.CanMove<<'\n'
    if (Player.IsDead) {
        DrawText("DEAD ", 400, 300, 50, RED);
    }
    if (Player.WallSliding) {
        DrawText("SLIDING", 300, 300, 20, GREEN);
    }

    if (Player.RayTouchingSL) {
        DrawText(" LEFT WALL", 150, 300, 20, GREEN);
    }
    if (Player.RayTouchingSR) {
        DrawText("RIGT WALL", 150, 300, 20, GREEN);
    }
    if (Player.WantToMoveL) {
        DrawText("want to move left", 150, 20, 20, WHITE);
    }
    if (Player.WantToMoveR) {
        DrawText("want to move Right", 150, 20, 20, WHITE);
    }
    if (Player.RememberJump > 0) {
        DrawText("want tRighto move ", 150, 20, 20, RED);
    }
    if (Player.RememberJump > 0) {
        DrawText("want tRighto move ", 150, 20, 20, RED);
    }
            for (int i = 0; i < frGetWorldBodyCount(Juego.f_world); i++) {
                frBody* body = frGetWorldBody(Juego.f_world, i);
                DrawRectangleLinesEx(frRecMetersToPixels(frGetBodyAABB(body)),1,GREEN);
               }
            frRaycastHit hitsC[300];
            frRaycastHit hitsR[300];
            frRaycastHit hitsL[300];

            int countC = frComputeWorldRaycast(Juego.f_world, Player.Rays[0], hitsC);
            int countR = frComputeWorldRaycast(Juego.f_world, Player.Rays[1], hitsR);
            int countL = frComputeWorldRaycast(Juego.f_world, Player.Rays[2], hitsL);
            if (countC > 1) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[0].origin),
                    frVec2MetersToPixels(hitsC[1].point),
                    1,
                    BLUE
                );
            }
            if (countR > 1) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[1].origin),
                    frVec2MetersToPixels(hitsR[1].point),
                    1,
                    BLUE
                );
            }
            if (countL > 1) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[2].origin),
                    frVec2MetersToPixels(hitsL[1].point),
                    1,
                    BLUE
                );
            }
            frRaycastHit hitsSR[300];
            frRaycastHit hitsSL[300];

            int countSR = frComputeWorldRaycast(Juego.f_world, Player.Rays[5], hitsSR);
            int countSL = frComputeWorldRaycast(Juego.f_world, Player.Rays[6], hitsSL);

            for (int i = 1; i < countSR; i++) {
                if (frGetBodyFlags(hitsSR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsSR[0].point, hitsSR[i].point) <= frNumberPixelsToMeters(2)) {
                    Player.RayTouchingSR = true;
                }
                else {
                    Player.RayTouchingSR = false;
                }
            }

            for (int i = 1; i < countSL; i++) {

                if (frGetBodyFlags(hitsSL[i].body) == FR_FLAG_WALL && frVec2Distance(hitsSL[0].point, hitsSL[i].point) <= frNumberPixelsToMeters(2)) {
                    Player.RayTouchingSL = true;
                }
                else {
                    Player.RayTouchingSL = false;
                }

            }

            for (int i = 1; i < countSR; i++) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[5].origin),
                    frVec2MetersToPixels(hitsSR[1].point),
                    1,
                    BLUE
                );
                if (frGetBodyFlags(hitsSR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsSR[0].point, hitsSR[i].point) <= frNumberPixelsToMeters(2)) {
                    DrawRing(frVec2MetersToPixels(hitsSR[i].point), 2, 3, 0, 360, 64, RED);
                    DrawRing(frVec2MetersToPixels(hitsSR[0].point), 2, 3, 0, 360, 64, RED);
                    DrawText("DERECHA", 400, 50, 20, GREEN);
                }
                if (Player.RayTouchingSR) {

                    DrawText(TextFormat("%f", (frVec2Distance(hitsSR[0].point, hitsSR[i].point))), 50, 50, 20, RED);
                }
            }

            if (countSL > 1) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[6].origin),
                    frVec2MetersToPixels(hitsSL[1].point),
                    1,
                    BLUE
                );
                if (frGetBodyFlags(hitsSL[1].body) == FR_FLAG_WALL && frVec2Distance(hitsSL[0].point, hitsSL[1].point) <= frNumberPixelsToMeters(2)) {
                    DrawRing(frVec2MetersToPixels(hitsSL[1].point), 2, 3, 0, 360, 64, RED);
                    DrawRing(frVec2MetersToPixels(hitsSL[0].point), 2, 3, 0, 360, 64, RED);
                    DrawText("IZQUERDA", 40, 50, 20, GREEN);
                }

                DrawText(TextFormat("%f", (frVec2Distance(hitsSL[0].point, hitsSL[1].point))), 50, 50, 20, GREEN);

            }
            Vector2 vel = frGetBodyVelocity(Player.Body);
            DrawText(TextFormat("velocityx %f ", vel.x), 0, -30, 20, WHITE);
            DrawText(TextFormat("velocityy %f ", vel.y), 300, -30, 20, WHITE);

}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        DrawMap();
        BeginMode2D(GetMapCamera());
        
        DrawPlayer();

       //debugmode();        
        
        EndMode2D();
             
        
        DrawText(TextFormat("LEVEL %i ", currentlvl), 30, 500, 50, WHITE);

        DrawText(TextFormat("TIME: %f ", GetElapsed(&Player.timer)), 400, 500, 50, WHITE);

        
        
    }
}game;

bool IsTransitionsPlaying = true;

bool firstTransition = 1;
bool deadTransition = 0;
bool levelTransition = 0;

int TransitionFrames = 0;
int TransitionTotalFrames = 26;


class TransitionScreen : public Screen {
public:
    
    void Draw() override {
        DrawMap();
        if (deadTransition) {
            if (TransitionFrames < TransitionTotalFrames) {
                DrawRectangle(0, 0, 800 , 600 / TransitionFrames, GRAY);
            }
            else {
                IsTransitionsPlaying = false;
                TransitionFrames = 0;
            }
        }
        else if (Player.levelPassed) {
            if (TransitionFrames < TransitionTotalFrames) {
                UpdateMusicStream(GetMusic(0));
                float x = int(840 - (800 / TransitionFrames));

                DrawRectangle(0, 0,x, 600 , RAYWHITE);

                DrawTriangle({ x, 0.f }, { x, 200.0f }, { x + 103.f, 100.0f }, RAYWHITE);
                DrawTriangle({ x, 200.f }, { x, 400.0f }, { x + 103.f, 300.0f }, RAYWHITE);
                DrawTriangle({ x, 400.f }, { x, 600.0f }, { x + 103.f, 500.0f }, RAYWHITE);

            }
            else {
                IsTransitionsPlaying = false;
                Player.levelPassed = false;
                TransitionFrames = 0;
            }
        }
        else if (firstTransition) {
            if (TransitionFrames < TransitionTotalFrames) {
                DrawRectangle(0,0,800/TransitionFrames,600,WHITE);
            }
            else {
                IsTransitionsPlaying = false;
                TransitionFrames = 0;
            }
        }
    }
    
}Transition;


void UpdateGame() {
    if (firstLoad) {
        currentlvl = 0;
        PushLevels();
        firstLoad = false;
    }
     PlayMusicStream(GetMusic(0));
     if (Player.levelPassed) {
         IsTransitionsPlaying = true;
         NextLevel();
     }
    if (!IsTransitionsPlaying) {
        
        TransitionFrames = 0;
        HideCursor();

        if (TimerDone(&Player.timer) || Player.IsDead) {
                const TileObject* Spawn = GetFirstMapObjectOfType("Spawn");
                frSetBodyPosition(Player.Body,frVec2PixelsToMeters({ Spawn->Bounds.x + (Spawn->Bounds.width / 2), Spawn->Bounds.y + (Spawn->Bounds.height / 2) }));
                frSetBodyVelocity(Player.Body, {0,0});
            Player.CanMove = Player.CanMoveRememberTime;
            
            Player.IsDead = false;
             IsTransitionsPlaying = true;
             deadTransition = true;
            StartTimer(&Player.timer,5.f);
        }
        else {
            UpdatePlayer(&Player,Juego.f_world);
        }

       
        UpdateMusicStream(GetMusic(0));
       // SetMusicVolume(GetMusic(0),0.30);
        //UpdateCameraPlayerBoundsPush(&GetMapCamera(),&Player,800,600);

        SetActiveScreen(&game);
        UpdatePhysics();
    
    }
    else {
        TransitionFrames++;
        
        if (Player.levelPassed) {
            TransitionTotalFrames = 10;
            UpdateMusicStream(GetMusic(0));
        }
        else if (firstTransition) {
            TransitionTotalFrames = 26;
        }
        else if (deadTransition && !Player.levelPassed) {
            TransitionTotalFrames = 10;
            UpdateMusicStream(GetMusic(0));
        }

        SetActiveScreen(&Transition);
    }

    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}

