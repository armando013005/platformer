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
    
    bool IsDead = false;
    bool levelPassed = false;

    float WallSlideSpeed = 0.00201f;
    float WallJumpForce = .08f;
    float WallJumpDirection = -1;
    Vector2 WallJumpAngle = {2.5,-2.3f};

    bool RayOnGroundC = false;
    bool RayOnGroundR = false;
    bool RayOnGroundL = false;

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

    float RememberJumpTime = 0.05f;
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