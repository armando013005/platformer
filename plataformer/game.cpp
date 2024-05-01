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
#include <iomanip> 
#include <memory>
#include <algorithm> 



#define PLAYER_MATERIAL   { 23.f, 0.0f, 0.0f, 0.f }

bool firstLoad = true;
int currentlvl = 0;
std::vector<const char*> niveles;

                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical,2 playaforma flotante)}

Entidad Player;
    
juego Juego;

void NextLevel() {
    currentlvl++;
    std::cout << currentlvl;
    if (niveles.empty() || niveles.size() <= currentlvl) { 
        Player.WIN= true; 
    }
    else {
    LoadMap(niveles[currentlvl]);

    Player.CanMove = Player.CanMoveRememberTime;
    StartTimer(&Player.timer,5.f);
    frSetBodyVelocity(Player.Body, {0,0});
    initphysics();
    }
      
}

void PushLevels() {
    niveles.push_back("nullptr");
    niveles.push_back("resources/levels/defaulttest.tmx");
    niveles.push_back("resources/levels/level4.tmx");
    niveles.push_back("resources/levels/level5.tmx");
    niveles.push_back("resources/levels/level3.tmx");
    niveles.push_back("resources/levels/jumplevel.tmx");
    niveles.push_back("resources/levels/buggedlvl.tmx");
    currentlvl = 0;

    NextLevel();
}
void initPads() {
    std::vector <const TileObject*> padU = GetMapObjectsOfType("PadU");
    std::vector <const TileObject*> padD = GetMapObjectsOfType("PadD");
    std::vector <const TileObject*> padR = GetMapObjectsOfType("PadR");
    std::vector <const TileObject*> padL = GetMapObjectsOfType("PadL");

    for (auto m : padU) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
            FR_BODY_STATIC,
            FR_FLAG_JUMPADU,
            frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width / 2), m->Bounds.y + (m->Bounds.height / 2) }),
            frCreateRectangle({ 1.25f, 0.0f, 0.0f, 0.0f }, frNumberPixelsToMeters(m->Bounds.width), frNumberPixelsToMeters(m->Bounds.height))
        );

        frAddToWorld(Juego.f_world, wal);
    }
    for (auto m : padD) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
            FR_BODY_STATIC,
            FR_FLAG_JUMPADD,
            frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width / 2), m->Bounds.y + (m->Bounds.height / 2) }),
            frCreateRectangle({ 1.25f, 0.0f, 0.0f, 0.0f }, frNumberPixelsToMeters(m->Bounds.width), frNumberPixelsToMeters(m->Bounds.height))
        );

        frAddToWorld(Juego.f_world, wal);
    }
    for (auto m : padR) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
            FR_BODY_STATIC,
            FR_FLAG_JUMPADR,
            frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width / 2), m->Bounds.y + (m->Bounds.height / 2) }),
            frCreateRectangle({ 1.25f, 0.0f, 0.0f, 0.0f }, frNumberPixelsToMeters(m->Bounds.width), frNumberPixelsToMeters(m->Bounds.height))
        );

        frAddToWorld(Juego.f_world, wal);
    }
    for (auto m : padL) {
        Player.maxP++;
        frBody* wal = frCreateBodyFromShape(
            FR_BODY_STATIC,
            FR_FLAG_JUMPADL,
            frVec2PixelsToMeters({ m->Bounds.x + (m->Bounds.width / 2), m->Bounds.y + (m->Bounds.height / 2) }),
            frCreateRectangle({ 1.25f, 0.0f, 0.0f, 0.0f }, frNumberPixelsToMeters(m->Bounds.width), frNumberPixelsToMeters(m->Bounds.height))
        );

        frAddToWorld(Juego.f_world, wal);
    }
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

    initPads();

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

    frSimulateWorld(Juego.f_world,GetFrameTime() * 100);

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
    std::cout << "TIME: " << GetElapsed(&Player.TotalTime)<< '\n';
    if (Player.IsDead) {
        DrawText("DEAD ", 300, 300, 20, RED);
    }
    switch (Player.pad)
    {
    case Player.Pad::PadU:
        DrawText("PadU ", 300, 300, 20, RED);
        break;
    case Player.Pad::PadD:
        DrawText("PadD ", 300, 300, 20, RED);
        break;
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
        DrawText("want tRighto move ", 150, 20, 70, RED);
    }

    if (Player.GroundedRemember > 0) {
        DrawText("want tRighto move ", 20, 20, 70, RED);
    }
    std::cout <<"canmove :" << Player.CanMove << '\n';

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
            for (int i = 1; i < countC; i++) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[0].origin),
                    frVec2MetersToPixels(hitsC[1].point),
                    1,
                    BLUE
                );
                if (frGetBodyFlags(hitsC[i].body) == FR_FLAG_WALL && frVec2Distance(hitsC[0].point, hitsC[i].point) <= frNumberPixelsToMeters(2)) {
                    DrawRing(frVec2MetersToPixels(hitsC[i].point), 2, 3, 0, 360, 64, RED);
                    DrawRing(frVec2MetersToPixels(hitsC[0].point), 2, 3, 0, 360, 64, RED);
                    DrawText("DERECHA", 400, 50, 20, GREEN);
                }
            }
            
            for (int i = 1; i < countR; i++) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[1].origin),
                    frVec2MetersToPixels(hitsR[1].point),
                    1,
                    BLUE
                );
                if (frGetBodyFlags(hitsR[i].body) == FR_FLAG_WALL && frVec2Distance(hitsR[0].point, hitsR[i].point) <= frNumberPixelsToMeters(2)) {
                    DrawRing(frVec2MetersToPixels(hitsR[i].point), 2, 3, 0, 360, 64, RED);
                    DrawRing(frVec2MetersToPixels(hitsR[0].point), 2, 3, 0, 360, 64, RED);
                    DrawText("DERECHA", 400, 50, 20, GREEN);
                }
            }
            for (int i = 1; i < countL; i++) {
                DrawLineEx(
                    frVec2MetersToPixels(Player.Rays[2].origin),
                    frVec2MetersToPixels(hitsL[1].point),
                    1,
                    BLUE
                );
                if (frGetBodyFlags(hitsL[i].body) == FR_FLAG_WALL && frVec2Distance(hitsL[0].point, hitsL[i].point) <= frNumberPixelsToMeters(2)) {
                    DrawRing(frVec2MetersToPixels(hitsL[i].point), 2, 3, 0, 360, 64, RED);
                    DrawRing(frVec2MetersToPixels(hitsL[0].point), 2, 3, 0, 360, 64, RED);
                    DrawText("DERECHA", 400, 50, 20, GREEN);
                }
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

struct TimeplusAnim
{
    Vector2 Pos = {0,0};
    int TFrames = 0;
    bool Done = false;
    int fontsize = 70;
    std::string text;
    int maxFrames=25;
};

std::vector<TimeplusAnim> anims;

void UpdateTextAnim() {
    for (auto i = 0; i < anims.size(); i++) {
        if (anims[i].TFrames >= anims[i].maxFrames) {
            anims[i].Done = true;
            anims.erase(anims.begin()+i);
        }
        else {
            anims[i].TFrames++;
            float p = 255 / anims[i].TFrames;
            DrawText(anims[i].text.c_str(), anims[i].Pos.x, anims[i].Pos.y, anims[i].fontsize / anims[i].TFrames, Fade(WHITE,p/100));
        }
    }
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
        
        DrawText(TextFormat("TIME: %f ", float(GetElapsed(&Player.timer))), 400, 500, 50, WHITE);

        if (Player.One_sec) {
            TimeplusAnim One_sec = {
                {GetRandomValue(100,700), 70},{0},{false},{70},{"+1"}
            };
            anims.push_back(One_sec);
        }
        if (Player.Half_sec) {
            TimeplusAnim One_sec = {
                {GetRandomValue(100,700), 70},{0},{false},{70},{"+0.5"}
            };
            anims.push_back(One_sec);
        }
        if (Player.Quarter_sec) {
            TimeplusAnim q_sec = {
                 {GetRandomValue(100,700), 70},{0},{false},{70},{"+0.25"}
            };
            anims.push_back(q_sec);
        }
        if (!anims.empty()) {
            UpdateTextAnim();
        }
        
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
                UpdateMusicStream(GetMusic(0));
                DrawRectangle(0, 0, 800 , 600 / TransitionFrames, GRAY);
            }
            else {
                IsTransitionsPlaying = false;
                TransitionFrames = 0;
            }
        }
        else if (Player.levelPassed) {
            if (TransitionFrames <= TransitionTotalFrames) {
                UpdateMusicStream(GetMusic(0));
                float x = int(800 - (800 / TransitionFrames));

                DrawRectangle(0, 0,x, 600 , RAYWHITE);

                DrawTriangle({ x, 0.f }, { x, 200.0f }, { x + 103.f, 100.0f }, RAYWHITE);
                DrawTriangle({ x, 200.f }, { x, 400.0f }, { x + 103.f, 300.0f }, RAYWHITE);
                DrawTriangle({ x, 400.f }, { x, 600.0f }, { x + 103.f, 500.0f }, RAYWHITE);

            }
            else {
                IsTransitionsPlaying = false;
                //Player.levelPassed = false;
                TransitionFrames = 0;
            }
        }
        else if (firstTransition) {
            if (TransitionFrames < TransitionTotalFrames) {
                float x = int(800 - (800 / TransitionFrames));

                DrawRectangle(0, 0, x, 600, RAYWHITE);

                DrawTriangle({ x, 0.f }, { x, 200.0f }, { x + 103.f, 100.0f }, RAYWHITE);
                DrawTriangle({ x, 200.f }, { x, 400.0f }, { x + 103.f, 300.0f }, RAYWHITE);
                DrawTriangle({ x, 400.f }, { x, 600.0f }, { x + 103.f, 500.0f }, RAYWHITE);
            }
            else {
                IsTransitionsPlaying = false;
                TransitionFrames = 0;
            }
        }
    }
    
}Transition;

struct TextAnim
{
    Vector2 Pos = { 0,0 };
    int TFrames = 0;
    bool Done = false;
    int fontsize = 50;
    std::string text;
    int maxFrames = 20;
    float num = 0;
};
std::vector<TextAnim> Tanims;

int currentText = 0;

void UpdateTextWinAnim() {
   // std::cout << "sex 3 \n" << currentText;
    if (!Tanims.empty() && (currentText+1 <= Tanims.size())) {
        std::cout << "sex 3 \n" << currentText;
        if (Tanims[currentText].TFrames >= Tanims[currentText].maxFrames) {
            Tanims[currentText].Done = true;
            PlaySound(GetSound(0));
            currentText++;
        }
        else {
            Tanims[currentText].TFrames++;
          
            float p = Tanims[currentText].fontsize / Tanims[currentText].TFrames;
            float n = Tanims[currentText].num / (20 - Tanims[currentText].TFrames);
            DrawCenteredText(Tanims[currentText].Pos.y, TextFormat(Tanims[currentText].text.c_str(), n), Tanims[currentText].fontsize - p , Fade(WHITE,p/10));
        }
    }

    for (auto i = 0; i < Tanims.size(); i++) {
        if (Tanims[i].Done) {
            if (i == 0) {
                DrawCenteredText(Tanims[i].Pos.y, TextFormat(Tanims[i].text.c_str(), int (Tanims[i].num)), Tanims[i].fontsize, WHITE);
            }
            else {
                DrawCenteredText(Tanims[i].Pos.y,TextFormat(Tanims[i].text.c_str(), Tanims[i].num), Tanims[i].fontsize, WHITE);
            }
        }
    }
}

void restart() {
    firstLoad = true;
    Tanims.erase(Tanims.begin() , Tanims.end());
    Player.deads = 0;
    //Player.WIN = false;
    Player.best_level_time = 6;
    Player.total_time = 0;
    ShowCursor();
    ResumeGame();
}
class WinScreen : public Screen {
public:
    void Draw() override {
        ShowCursor();
        DrawCenteredText(50,"YOU WON",50, WHITE);
        UpdateTextWinAnim();

        /*if (DrawCenteredButton(400, 70, 40, "RESTART")) {
            PlaySound(GetSound(3));
            restart();

        }*/

        if (DrawCenteredButton(450, 70, 40, "QUIT TO MAIN MENU")) {
            PlaySound(GetSound(3));
            gotomenu();
        }
        //DrawCenteredText(200, TextFormat("DEADS: %i", Player.deads), 50, WHITE);
    }
}Winscreen;

void UpdateGame() {


    if (firstLoad) {
        currentlvl = 0;
        if (!Player.WIN) {

            PushLevels();
        }
        StartTimer(&Player.TotalTime,0);
        Player.deads = 0;
        Player.WIN = false;
        Player.best_level_time = 1000;
        Player.total_time = 0;

        firstLoad = false;
    }

    if (!Player.WIN) {

         PlayMusicStream(GetMusic(0));

         if (Player.levelPassed) {
             IsTransitionsPlaying = true;
             Player.levelPassed = false;
             Player.lvltrans = true;
             //firstTransition = false;
             Player.best_level_time = std::min(Player.best_level_time, GetElapsed(&Player.timer));
             NextLevel();
         }
         else {
             Player.lvltrans = true;
         }

        if (!IsTransitionsPlaying) {
        
            TransitionFrames = 0;
            HideCursor();
            if (Player.One_sec) {
                if ((GetElapsed(&Player.timer) - 1) <= 0) {
                    Player.timer.StartTime = GetTime();
                    Player.One_sec = false;
                }
                else {
                    Player.timer.StartTime += 1;
                    Player.One_sec = false;
                }
            }
            else if (Player.Half_sec) {
                if ((GetElapsed(&Player.timer) - 0.5) <= 0) {
                    Player.timer.StartTime = GetTime();
                    Player.Half_sec = false;
                }
                else {
                    Player.timer.StartTime += 0.5;
                    Player.Half_sec = false;
                }
            }else if (Player.Quarter_sec) {
                if ((GetElapsed(&Player.timer) - 0.25) <= 0) {
                    Player.timer.StartTime = GetTime();
                    Player.Quarter_sec = false;
                }
                else {
                    Player.timer.StartTime += 0.25;
                    Player.Quarter_sec = false;
                }
            }

        
        
            if (TimerDone(&Player.timer) || Player.IsDead) {
                    const TileObject* Spawn = GetFirstMapObjectOfType("Spawn");
                    frSetBodyPosition(Player.Body,frVec2PixelsToMeters({ Spawn->Bounds.x + (Spawn->Bounds.width / 2), Spawn->Bounds.y + (Spawn->Bounds.height / 2) }));
                    frSetBodyVelocity(Player.Body, {0,0});
                Player.CanMove = Player.CanMoveRememberTime;
                PlaySound(GetSound(1));
                Player.IsDead = false;
                 IsTransitionsPlaying = true;
                 deadTransition = true;
                 Player.deads++;
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
            if (deadTransition && Player.levelPassed) {
                TransitionTotalFrames = 10;
                deadTransition = false;
                UpdateMusicStream(GetMusic(0));
            }
            if (Player.lvltrans) {
                TransitionTotalFrames = 10;
                UpdateMusicStream(GetMusic(0));
            }
            else if (firstTransition) {
                TransitionTotalFrames = 10;
            }
            else if (deadTransition && !Player.levelPassed) {
                TransitionTotalFrames = 10;
                UpdateMusicStream(GetMusic(0));
            }

            SetActiveScreen(&Transition);
        }
    }
    else {
        if (Player.total_time == 0) {
            Player.total_time = GetElapsed(&Player.TotalTime);
            std::cout << "sex6 " << GetElapsed(&Player.TotalTime) << '\n';
            
            TextAnim dead = { {0,200},0,0,50,"DEADS: %i ",20,Player.deads};
            TextAnim BestTime = { {0,270},0,0,50,"BEST TIME: %f",20,Player.best_level_time };
            TextAnim TotalTime = { {0,350},0,0,50,"TOTAL TIME: %f Sec(s)",20,Player.total_time };
            /*struct TextAnim
                {
                    Vector2 Pos = { 0,0 };
                    int TFrames = 0;
                    bool Done = false;
                    int fontsize = 50;
                    std::string text;
                    int maxFrames = 20;
                    float num = 0;
                };
            */
            Tanims.push_back(dead);
            Tanims.push_back(BestTime);
            Tanims.push_back(TotalTime);

        }
        SetActiveScreen(&Winscreen);
    }

    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();   
}

