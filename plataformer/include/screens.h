#pragma once
#include "raylib.h"

class Screen
{
public:
	virtual void Draw() = 0;
protected:
	void DrawCenteredText(int y, const char* text, int fontSize, Color color);

};

void SetActiveScreen(Screen* screen);

void DrawScreen();