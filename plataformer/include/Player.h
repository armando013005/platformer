#pragma once
#include "raylib.h"
#include "include/game.h"
#include "map.h"
#include "ferox.h"
#include "include/timer.h"
class Entidad
{
public:
    int texture = 300;
    Color color = RED;
    frBody* Body;

    Rectangle Exit = {0,0,0,0};

    Timer timer;

    float CanMoveRememberTime = 0.1f;
    float CanMove = 0;
    
    bool One_sec = false;
    bool Half_sec = false;
    bool Quarter_sec = false;

    bool IsDead = false;
    bool levelPassed = false;

    float WallSlideSpeed = 0.00201f;
    float WallJumpForce = .08f;
    float WallJumpDirection = 1;
    Vector2 WallJumpAngle = {2.5,-2.3f};

    typedef enum Pad {
        PadU,
        PadD,

        None
    };
   
    Pad pad = Pad::None;

    int deads = 0;
    float best_level_time = 6;
    float total_time = 0;
    Timer TotalTime;
    
    bool WIN = false;

    bool RayOnGroundC = false;
    bool RayOnGroundR = false;
    bool RayOnGroundL = false;

    float PadTime = 0;
    float PadRememberTime = 1.0f;

    bool lvltrans = false;

    bool RayTouchingR = false;
    bool RayTouchingL = false;
    bool canmove = true;

    bool WantToMoveL = false;
    bool WantToMoveR = false;

    bool TouchingWall = false;
    bool WallSliding = false;


    bool RayTouchingSR = false;
    bool RayTouchingSL = false;

    int maxP;

    float WallRememberTime = 0.09f;
    float WallRemember = 0;

    float GroundedRememberTime = 0.09f;
    float GroundedRemember = 0;

    float RememberJumpTime = 0.09f;
    float RememberJump = 0;

    Vector2 Player_Pos = frGetBodyPosition(Body);

    frRay Rays[7]{
        //center
         {{Player_Pos.x,Player_Pos.y + frNumberPixelsToMeters(2)},{0,Player_Pos.y + frNumberPixelsToMeters(1)}, 2.f},
        //right
         {{Player_Pos.x + (frGetBodyAABB(Body).width / 2) ,Player_Pos.y + frNumberPixelsToMeters(2)},{0,Player_Pos.y}, 2.f},
        //left
         {{Player_Pos.x - (frGetBodyAABB(Body).width / 2),Player_Pos.y + frNumberPixelsToMeters(2)},{0,Player_Pos.y },2.f},
         //toprigt
        {{Player_Pos.x + (frGetBodyAABB(Body).width / 2) ,Player_Pos.y + frNumberPixelsToMeters(2)},{0,-Player_Pos.y}, 2.f},
        //topleft
        {{Player_Pos.x - (frGetBodyAABB(Body).width / 2),Player_Pos.y + frNumberPixelsToMeters(2)},{0,-Player_Pos.y },2.f},
        //siderigt
        {{Player_Pos.x + (frGetBodyAABB(Body).width / 2) ,Player_Pos.y} ,{Player_Pos.x,0}, 2.f},
        //sideleft
        {{Player_Pos.x - (frGetBodyAABB(Body).width / 2),Player_Pos.y },{-Player_Pos.x,0 }, 2.f}
    };

private:

};






void UpdatePlayer(Entidad* Player, frWorld* world);

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Entidad* player, int width, int height);