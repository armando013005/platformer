#pragma once
#include "raylib.h"

class Screen
{
public:
	virtual void Draw() = 0;
protected:
	void DrawCenteredText(int y, const char* text, int fontSize, Color color);
	bool DrawCenteredButton(float y, float width, float height, const char* text);
	void DrawInGameMenu();

};

void SetActiveScreen(Screen* screen);

void DrawScreen();