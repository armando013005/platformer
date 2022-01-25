#define RAYGUI_IMPLEMENTATION
#include "include/main.h"
#include <raylib.h>
#include "include/raygui.h"

#include "include/loading.h"
#include "include/game.h"
#include "include/pause.h"
#include "include/screens.h"
#include "include/map.h"
#include "include/tile_map.h"

int windowWith = 800;
int windowHeight = 600;
int framecounter = 0;
float musicVolume = 0.3f;
enum class ApplicationStates
{
    Startup,
    Loading,
    Menu,
    Running,
    Paused,
    Quiting,
    MoreOptions
} ApplicationState;

/*class StarupScreen : public Screen
{
public:
    void Draw() override {
        Color blanco = { 245, 245, 245, 255 - framecounter };
        DrawCenteredTextEx(GetScreenHeight() / 2, "Made by Armando Orozco\n     @armando013005", 40, blanco, GetFont(0));

    }
}starupScreen;
*/
class OptionsMenu : public Screen {

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

        Rectangle o = { 800.f / 2.f - 70.f / 2.f, rectangle.y - 40.f / 2.f, 70.f, 40.f };

        Rectangle ou = { 800.f / 2.f - 70.f / 2.f, rectangle.y + 120.f / 2.f, 70.f, 20.f };

        GuiDummyRec(o, "OPTIONS");
        DrawRectangleLinesEx(o, 3, RAYWHITE);
      
         musicVolume = GuiSliderBar(ou, TextFormat("VOLUME : %i",int(musicVolume*100)), "100", musicVolume, 0, 1);
        SetMasterVolume(musicVolume);

        if (DrawCenteredButton(600 - 600 / 2, 70, 40, "BACK")) {
            PlaySound(GetSound(3));
            ResumeGame(); 
            
        }

        if (DrawCenteredButton(600 - 600 / 2.6, 70, 40, "QUIT TO MAIN MENU")) {
            PlaySound(GetSound(3));
            gotomenu();
        } 

    }

}options;

class MainMenu : public Screen{
public:
    void Draw() override {

        //ashesfont = LoadFontEx("styles / ashes / v5loxical.ttf", 32, 0, 250);
        //DrawCenteredTextEx(600 / 3, "GAME NAME", 40, RAYWHITE, GetFont(0));
        DrawCenteredText(GetScreenHeight() / 3, "Close Time", 50, RAYWHITE);

        
        if (DrawCenteredButton(600 / 2, 100, 30, "PLAY")) {
            PlaySound(GetSound(3));

            ApplicationState = ApplicationStates::Running;
       
        }

        if (DrawCenteredButton(600 - 600 / 2.5, 70, 30, "QUIT")) {
            PlaySound(GetSound(3));
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

void LoadComplete()
{
    ApplicationState = ApplicationStates::Menu;
    

}

void SetupWindow()
{
    //funcion robadita del rpg example xdd https://github.com/raylib-extras/RPGExample/blob/main/RPG/main.cpp

    SetWindowMinSize(800, 600);
    SetExitKey(0);
    SetTargetFPS(60);

    // load an image for the window icon
    Image icon = LoadImage("resources/Tiles/Default/tile_0300.png");

    // ensure that the picture is in the correct format
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    // replace the background and border colors with transparent
    /*ImageColorReplace(&icon, BLACK, BLANK);
    ImageColorReplace(&icon, Color{ 136,136,136,255 }, BLANK);*/

    // set the icon
    SetWindowIcon(icon);

    // free the image data
    UnloadImage(icon);
}


/*void UpdateStartup() {

    ApplicationState = ApplicationStates::Startup;

    if (GetTime() >= 3.f) {
        gotomenu();
    }
    else {
        SetActiveScreen(&starupScreen);
    }
    
}
*/

void StartGame() {
    ApplicationState = ApplicationStates::Running;
    
}

void MoreOptions() {
    ApplicationState = ApplicationStates::MoreOptions;
    SetActiveScreen(&options);
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(windowWith, windowHeight, "Plataformer");

    SetupWindow();
    InitAudioDevice();

    int gameScreenWidth = 800;
    int gameScreenHeight = 600;

    RenderTexture2D target = LoadRenderTexture(800, 600);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitResourses();
    
    ApplicationState = ApplicationStates::Loading;

    SetTargetFPS(60);
    SetMasterVolume(musicVolume);
    SetExitKey(NULL);

    while (!WindowShouldClose() && ApplicationState != ApplicationStates::Quiting)    // Detect window close button or ESC key
    {
        // Update ----------------------------------------------------------------------------

        float scale = std::min((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);

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
        case ApplicationStates::MoreOptions:
            MoreOptions();
            break;
        }
        //----------------------------------------------------------------------------------
        framecounter++;
        // Draw
        BeginTextureMode(target);
        ClearBackground(BLACK);

        DrawScreen();


        EndTextureMode();

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTexturePro(target.texture,
            Rectangle{ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            Rectangle{ (GetScreenWidth() - ((float)gameScreenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * scale)) * 0.5f, (float)gameScreenWidth * scale, (float)gameScreenHeight * scale },
            Vector2{ 0, 0 },
            0.0f,
            WHITE);

        EndDrawing();
        
        //----------------------------------------------------------------------------------
        
    }

    // De-Initialization

    UnloadAll();
   
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();
    CloseWindow();      // Close window and audio devivce  and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

