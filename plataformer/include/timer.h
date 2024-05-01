#pragma once
#include "raylib.h"

typedef struct Timer
{
    float StartTime;
    float Lifetime;
};

void StartTimer(Timer* timer, float lifetime);

bool TimerDone(Timer* timer);

float GetElapsed(Timer* timer);