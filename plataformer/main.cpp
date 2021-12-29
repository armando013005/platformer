#define RAYGUI_IMPLEMENTATION
#include "include/main.h"
#include <raylib.h>
#include "include/raygui.h"

#include "include/loading.h"
#include "include/game.h"
#include "include/pause.h"
#include "include/screens.h"

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
        Color blanco = { 245, 245, 245, 255 - framecounter };
        DrawCenteredTextEx(GetScreenHeight() / 2, "MADE WITH RAYLIB", 50, blanco, GetFont(0));

    }
}starupScreen;

class MainMenu : public Screen{
public:
    void Draw() override {

        //ashesfont = LoadFontEx("styles / ashes / v5loxical.ttf", 32, 0, 250);
        DrawCenteredTextEx(GetScreenHeight() / 3, "GAME NAME", 50, RAYWHITE, GetFont(0));
        //DrawCenteredText(GetScreenHeight() / 3, "GAME NAME", 50, RAYWHITE);
        Rectangle boton = { 10,10,50,50 };

        if (DrawCenteredButton(GetScreenHeight() / 2, 100, 30, "PLAY")) {
            ApplicationState = ApplicationStates::Running;
        }

        if (DrawCenteredButton(GetScreenHeight() - GetScreenHeight() / 2.5, 70, 30, "QUIT")) {
            QuitApp();
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

    ApplicationState = ApplicationStates::Startup;

    if (GetTime() >= 2) {
        gotomenu();
    }
    else {
        SetActiveScreen(&starupScreen);
    }
    
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
    
    ApplicationState = ApplicationStates::Loading;

    SetTargetFPS(144);
   
    SetExitKey(NULL);

    while (!WindowShouldClose() && ApplicationState != ApplicationStates::Quiting)    // Detect window close button or ESC key
    {
        // Update ----------------------------------------------------------------------------

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

    UnloadAll();
   
    /*
    UnloadFont(fuentes[0]);
    UnloadTexture(sprites[0]);
    */
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();      // Close window and audio devivce  and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

