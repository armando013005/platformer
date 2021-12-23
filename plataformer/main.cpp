#define RAYGUI_IMPLEMENTATION
#include <raylib.h>
#include "include/raygui.h"

#include "include/game.h"
#include "include/main.h"
#include "include/pause.h"
#include "include/screens.h"


int windowWith = 1280;
int windowHeight = 720;
int framecounter = 0;

Font ashesfont;

enum class ApplicationStates
{
    Startup,
    Loading,
    Menu,
    Running,
    Paused,
    Quiting
} ApplicationState;

void QuitApp() {
    ApplicationState = ApplicationStates::Quiting;
}

class StarupScreen : public Screen
{
public:
    void Draw() override {
        Color blanco = { 245, 245, 245, 255-framecounter };
        DrawCenteredTextEx(GetScreenHeight()/2, "MADE WITH RAYLIB", 50, blanco, ashesfont);

    }
}starupScreen;


class MainMenu : public Screen
{
public:
    void Draw() override {

        //ashesfont = LoadFontEx("styles / ashes / v5loxical.ttf", 32, 0, 250);
        DrawCenteredTextEx(GetScreenHeight() / 3, "GAME NAME", 50, RAYWHITE, ashesfont);
        //DrawCenteredText(GetScreenHeight() / 3, "GAME NAME", 50, RAYWHITE);
        Rectangle boton = { 10,10,50,50 };

        if (DrawCenteredButton(GetScreenHeight()/2, 100, 30, "PLAY")) {
            ApplicationState = ApplicationStates::Running;
        }

        if (DrawCenteredButton(GetScreenHeight() - GetScreenHeight() / 2.5, 70, 30, "QUIT")) {
            QuitApp();
        }

    }
}mainMenu;


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

void StartGame() {
    ApplicationState = ApplicationStates::Running;
    
}

void UpdateMenu() {
    SetActiveScreen(&mainMenu);
}

void PauseGame() {
    ApplicationState = ApplicationStates::Paused;
}

void ResumeGame() {
    ApplicationState = ApplicationStates::Running;
    SetActiveScreen(nullptr);
}

int main() {
	
    

	InitWindow(windowWith, windowHeight, "Plataformer");
    InitAudioDevice();

    GuiLoadStyle("styles/ashes/ashes.rgs");
    ashesfont = LoadFontEx("styles / ashes / v5loxical.ttf", 32, 0, 250);
    
    
    ApplicationState = ApplicationStates::Startup;

    SetTargetFPS(144);
   
    SetExitKey(NULL);

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
            UpdatePause();
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
    UnloadFont(ashesfont);
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();      // Close window and audio devivce  and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}