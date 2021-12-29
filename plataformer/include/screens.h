#pragma once
#include "raylib.h"

class Screen
{
public:
	virtual void Draw() = 0;
protected:
	void DrawCenteredText(int y, const char* text, int fontSize, Color color);
	bool DrawCenteredButton(float y, float width, float height, const char* text);
	void DrawCenteredTextEx(int y, const char* text, int fontSize, Color color, Font font);
};

void SetActiveScreen(Screen* screen);

void DrawScreen();