#include <raylib.h>
#include "include/raygui.h"
#include "include/pause.h"

#include "include/screens.h"
#include "include/main.h"
#include "include/map.h"

class PauseMenu : public Screen {

public:
	void Draw() override {

		DrawMap();

		Color blanco = { 245, 245, 245, 100 };

		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), blanco);
		
		float width = GetScreenWidth() / 2;
		float Height = GetScreenHeight() / 2;
		Rectangle rectangle = { GetScreenWidth() / 2 - width / 2, GetScreenHeight() / 2 - Height / 2, width, Height };

		DrawRectangleRec(rectangle, BLACK);
		DrawRectangleLinesEx(rectangle, 3, RAYWHITE);

		Rectangle o = { GetScreenWidth()/2 - 70 /2, rectangle.y - 40/2, 70, 40 };

		GuiDummyRec(o, "OPTIONS");
		DrawRectangleLinesEx(o, 3, RAYWHITE);

		if (DrawCenteredButton(GetScreenHeight() / 2.6, 70, 40, "RESUME")) ResumeGame();

		if (DrawCenteredButton(GetScreenHeight() - GetScreenHeight() / 2, 70, 40, "QUIT TO MAIN MENU")) gotomenu();
	
		if (DrawCenteredButton(GetScreenHeight() - GetScreenHeight() / 2.6, 70, 40, "QUIT TO DESKTOP")) QuitApp();
		

	}

}pauseMenu;

void UpdatePause() {

	SetActiveScreen(&pauseMenu);
	
	if (IsKeyPressed(KEY_ESCAPE)) {
		ResumeGame();
	}


}