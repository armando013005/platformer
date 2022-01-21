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
#include <unordered_map>

#define PLAYER_MATERIAL   { 15.f, 0.0f, 1.0f, 1.f }

enum class ActorStates {
    idle,
    run,
    jumpUp,
    jumpSides
};

struct Entidad
{

    ActorStates state = ActorStates::idle;
    bool FacingRight = true;

    AnimSpriteInstance sprite;
    std::unordered_map<ActorStates, std::vector<SpriteAnimation>> AnimationStates;

    frBody* Body;


    bool canJump = false;

}Player;

bool firstLoad = true;
int currentlvl;
std::vector<const char*> niveles;
                        //solo un int
// {x,y,width,heigth},(0 horizontal, 1 vertical,2 playaforma flotante)}

    
juego Juego;


void UpdatePlayer(Entidad* Player) {

    frSetBodyRotation(Player->Body, 0);
    Player->canJump = true;

    if (IsKeyDown(KEY_RIGHT)) frApplyImpulse(Player->Body, { 0.02f,0.0f });
    if (IsKeyDown(KEY_LEFT)) frApplyImpulse(Player->Body, { -0.02f,0.f });
    if (IsKeyPressed(KEY_X) && Player->canJump) {
        Player->canJump = false;
        frApplyImpulse(Player->Body, { 0.0f,-0.2 });

    }



}


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

    SpriteSheet anim = GetSpriteSheet();
    AnimSpriteInstance ai = { {0,0}, Vector2{anim.Frames[0].width / 2, anim.Frames[0].height / 2 }, &anim };
    Player.sprite = ai;

    Player.AnimationStates[ActorStates::idle].emplace_back(SpriteAnimation{"idle",0,0,1,1});

    SetSpriteAnimation(Player.sprite,Player.AnimationStates[ActorStates::idle][0]);
    /*typedef struct SpriteAnimation
    {
        std::string Name;
        int StartFrame = -1;
        int EndFrame = -1;
        float FPS = 3;
        bool Loops = true;
    };*/
    /*struct AnimSpriteInstance
    {
    Vector2 Position = { 0 };

    Vector2 Offset = { 0,0 };
    const SpriteSheet* Sheet = nullptr;
    const SpriteAnimation* Animation = nullptr;
    bool AnimationDone = false;
    int CurrentFrame = -1;
    float FrameLifetime = 0;
    };*/
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
    Player.sprite.Position = pos;
    AnimDrawSprite(Player.sprite);

}

class GameScreen : public Screen {
public:
    void Draw() override {

        
        DrawMap();
        BeginMode2D(GetMapCamera());
        
        DrawPlayer();
        
        for (int i = 0; i < frGetWorldBodyCount(Juego.f_world); i++) {
            frBody* body = frGetWorldBody(Juego.f_world, i);

            frDrawBodyAABB(body,GREEN);
        }
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

