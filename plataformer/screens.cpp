#include "include/screens.h"
#include "include/raygui.h"
#include "include/main.h"
//#include "game.cpp"

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

void Screen::DrawCenteredTextEx(int y, const char* text, int fontSize, Color color, Font font)
{
	float spacing = fontSize / 10;

	Vector2 textWidth = MeasureTextEx(font, text, fontSize, spacing);

	Vector2 posicion = { GetScreenWidth() / 2 - textWidth.x / 2 ,y - fontSize / 2 };

	DrawTextEx(font, text, posicion, fontSize, spacing, color);
	//DrawText(text, GetScreenWidth() / 2 - textWidth.x / 2, y - fontSize / 2, fontSize, color);
}

bool Screen::DrawCenteredButton(float y, float width, float height, const char* text)
{
	if (width < MeasureText(text,10)) {
		width += MeasureText(text, 10);
	}
	Rectangle boton = { GetScreenWidth() / 2 - width / 2, y - height / 2, width, height };

	return GuiButton(boton, text);

}