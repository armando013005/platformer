#pragma once
#include "raylib.h"

typedef struct Timer
{
    double StartTime;
    double Lifetime;
};

void StartTimer(Timer* timer, double lifetime)
{
    timer->StartTime = GetTime();
    timer->Lifetime = lifetime;
}

bool TimerDone(Timer* timer)
{
    return GetTime() - timer->StartTime >= timer->Lifetime;
}

double GetElapsed(Timer* timer)
{
    return GetTime() - timer->StartTime;
}