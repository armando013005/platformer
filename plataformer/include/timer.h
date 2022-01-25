#pragma once
#include "raylib.h"

typedef struct Timer
{
    double StartTime;
    double Lifetime;
};

void StartTimer(Timer* timer, double lifetime);

bool TimerDone(Timer* timer);

double GetElapsed(Timer* timer);