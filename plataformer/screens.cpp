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

void Screen::DrawInGameMenu() {
	Color blanco = { 245, 245, 245, 100};

	DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),blanco);
/*
	float x;                // Rectangle top-left corner position x
	float y;                // Rectangle top-left corner position y
	float width;            // Rectangle width
	float height;
*/
	float width = GetScreenWidth() / 2;
	float Height = GetScreenHeight() / 2;
	Rectangle rectangle = { GetScreenWidth() / 2 - width / 2, GetScreenHeight() / 2 - Height / 2, width, Height };

	DrawRectangleRec(rectangle,BLACK);
	DrawRectangleLinesEx(rectangle, 3, RAYWHITE);
	if (DrawCenteredButton(GetScreenHeight() / 2.5, 70, 40, "RESUME")) {} //triggerMenu = !triggerMenu;

	if (DrawCenteredButton(GetScreenHeight() / 3.5, 70, 40, "QUIT TO MAIN MENU")) {
		QuitApp();
	}
}

void Screen::DrawCenteredText(int y, const char* text, int fontSize, Color color)
{
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, GetScreenWidth() / 2 - textWidth / 2, y - fontSize / 2, fontSize, color);
}

bool Screen::DrawCenteredButton(float y, float width, float height, const char* text)
{
	if (width < MeasureText(text,10)) {
		width += MeasureText(text, 10);
	}
	Rectangle boton = { GetScreenWidth() / 2 - width / 2, y - height / 2, width, height };

	return GuiButton(boton, text);

}