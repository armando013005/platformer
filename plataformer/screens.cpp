#include "include/screens.h"
#include "include/raygui.h"
Screen* ActiveScreen = nullptr;

void SetActiveScreen(Screen* screen)
{
	ActiveScreen = screen;
}

void DrawScreen()
{
	if (ActiveScreen != nullptr)
		ActiveScreen->Draw();
}

void Screen::DrawCenteredText(int y, const char* text, int fontSize, Color color)
{
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, GetScreenWidth() / 2 - textWidth / 2, y - fontSize / 2, fontSize, color);
}

bool Screen::DrawCenteredButton(float y, float height, const char* text, int fontSize)
{
	
	float buttonWidth = MeasureText(text, fontSize);
	
	Rectangle boton = { GetScreenWidth() / 2 - buttonWidth/2, y-fontSize, buttonWidth, height };

	return GuiButton(boton, text);

}