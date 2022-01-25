#include "include/timer.h"
#include "raylib.h"
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