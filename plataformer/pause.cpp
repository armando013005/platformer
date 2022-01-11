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

		DrawRectangle(0, 0, 800, 600, blanco);
		
		float width = 800 / 2;
		float Height = 600 / 2;
		Rectangle rectangle = { 800 / 2 - width / 2, 600 / 2 - Height / 2, width, Height };

		DrawRectangleRec(rectangle, BLACK);
		DrawRectangleLinesEx(rectangle, 3, RAYWHITE);

		Rectangle o = { 800.f/2.f - 70.f /2.f, rectangle.y - 40.f/2.f, 70.f, 40.f };

		GuiDummyRec(o, "OPTIONS");
		DrawRectangleLinesEx(o, 3, RAYWHITE);

		if (DrawCenteredButton(600 / 2.6, 70, 40, "RESUME")) ResumeGame();

		if (DrawCenteredButton(600 - 600 / 2, 70, 40, "QUIT TO MAIN MENU")) gotomenu();
	
		if (DrawCenteredButton(600 - 600 / 2.6, 70, 40, "QUIT TO DESKTOP")) QuitApp();
		

	}

}pauseMenu;

void UpdatePause() {

	SetActiveScreen(&pauseMenu);
	
	if (IsKeyPressed(KEY_ESCAPE)) {
		ResumeGame();
	}


}