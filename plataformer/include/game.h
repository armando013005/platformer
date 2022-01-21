#pragma once
#include <raylib.h>
#include "include/Player.h"
#include "ferox.h"

void NextLevel();

void DrawPlayer();

void PushLevels();

void UpdateGame();

class juego
{
public:
	frWorld* f_world;
private:


};
void initphysics();
void UpdatePhysics();


 