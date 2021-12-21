#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include "include/game.h"
#include "include/screens.h"
#include "include/raygui.h"

int windowWith = 800;
int windowHeight = 600;
int framecounter = 0;

enum class ApplicationStates
{
    Startup,
    Loading,
    Menu,
    Running,
    Paused,
    Quiting
} ApplicationState;

class StarupScreen : public Screen
{
public:
    void Draw() override {
        Color blanco = { 245, 245, 245, 255-framecounter };
        DrawCenteredText(windowHeight/2, "MADE WITH RAYLIB", 50, blanco);

    }
}starupScreen;


class MainMenu : public Screen
{
public:
    void Draw() override {


        DrawCenteredText(windowHeight / 4, "GAME NAME", 50, RAYWHITE);
        Rectangle boton = { 10,10,50,50 };
        if (DrawCenteredButton(GetScreenHeight()/2, 300, "PLAY", 300)) {
            DrawCenteredText(windowHeight / 2, "BUTOOOOON", 50, RAYWHITE);
        }
        

    }
}mainMenu;

void QuitApp() {
    ApplicationState = ApplicationStates::Quiting;
}

void gotomenu() {
    ApplicationState = ApplicationStates::Menu;
    SetActiveScreen(&mainMenu);
}

void UpdateStartup() {
    if (GetTime() >= 2) {
        gotomenu();
    }
    else {
        SetActiveScreen(&starupScreen);
    }
}

void UpdateLoad() {

}



void UpdateMenu() {
    //ApplicationState = ApplicationStates::Menu;
    //SetActiveScreen(&mainMenu);
    if (IsKeyDown(KEY_ESCAPE)) {
        QuitApp();
    }
}

void UpdatePaused() {
    
}

void main() {
	
    

	InitWindow(windowWith, windowHeight, "Plataformer");
    GuiLoadStyle("/styles/ashes/ashes.rgs");

    ApplicationStates ApplicationState = ApplicationStates::Startup;

    SetTargetFPS(144);
   

    while (!WindowShouldClose() && ApplicationState != ApplicationStates::Quiting)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (ApplicationState)
        {
        case ApplicationStates::Loading:
            UpdateLoad();
            break;

        case ApplicationStates::Menu:
            UpdateMenu();
            break;

        case ApplicationStates::Running:
            UpdateGame();
            break;

        case ApplicationStates::Paused:
            UpdatePaused();
            break;
        case ApplicationStates::Startup:
            UpdateStartup();
            break;
        }
        //----------------------------------------------------------------------------------
        framecounter++;
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawScreen();

        EndDrawing();
        
        //----------------------------------------------------------------------------------
        
       
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------


}