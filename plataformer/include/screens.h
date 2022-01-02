#pragma once
#include "raylib.h"

class Screen
{
public:
	virtual void Draw() = 0;
	
};

void DrawCenteredText(int y, const char* text, int fontSize, Color color);
void DrawCenteredTextEx(int y, const char* text, int fontSize, Color color, Font font);
bool DrawCenteredButton(float y, float width, float height, const char* text);

void SetActiveScreen(Screen* screen);

void DrawScreen();