#include "include/timer.h"
#include "raylib.h"
void StartTimer(Timer* timer, float lifetime)
{
    timer->StartTime = float(GetTime());
    timer->Lifetime = lifetime;
}

bool TimerDone(Timer* timer)
{
    return GetTime() - timer->StartTime >= timer->Lifetime;
}

float GetElapsed(Timer* timer)
{
    return GetTime() - timer->StartTime;
}