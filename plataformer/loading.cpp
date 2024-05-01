#include "include/main.h"
#include <raylib.h>
#include "include/screens.h"
#include "include/loading.h"
#include "include/raygui.h"
#include "include/sprites.h"
#include <vector>
#include <deque>
#include <string>

int LoadedItems, TotalToLoad;

class LoadingScreen : public Screen
{
public:
    std::string LoadingText = "Loading...";

    LoadingScreen()
    {
        int size = MeasureText(LoadingText.c_str(), 20);
        Origin.x = 800 * 0.5f - size * 0.5f;
        Origin.y = 600 * 0.5f - 10;

        LeftSpinner.x = Origin.x - 25.0f;
        RightSpinner.x = Origin.x + size + 25.0f;
        LeftSpinner.y = RightSpinner.y = 600 * 0.5f;

        LeftSpinner.width = RightSpinner.width = 20;
        LeftSpinner.height = RightSpinner.height = 20;
    }

    void Draw() override
    {
        // tell them we are loading
        DrawText(LoadingText.c_str(), int(Origin.x), int(Origin.y), 20, WHITE);

        // some spinny things to know that the app hasn't locked up
        DrawRectanglePro(LeftSpinner, Vector2{ 10, 10 }, float(GetTime()) * 180.0f, WHITE);
        DrawRectanglePro(RightSpinner, Vector2{ 10, 10 }, float(GetTime()) * -180.0f, WHITE);

        // progress bar.
        float progressWidth = RightSpinner.x - LeftSpinner.x;
        DrawRectangle(int(LeftSpinner.x), int(LeftSpinner.y + 20), (int)(progressWidth * Progress), 5, BLACK);
    }

    Vector2 Origin = { 0,0 };

    Rectangle LeftSpinner = { 0,0 };
    Rectangle RightSpinner = { 0,0 };

    // Load progress 0 = 0% 1 = 100%
    float Progress = 0;
};

LoadingScreen* LoadScreen = nullptr;
std::deque<std::string> TtoLoad;
std::deque<std::string> StoLoad;
std::deque<std::string> MtoLoad;

std::vector<Font> fuentes;
std::vector<Texture2D> sprites;
std::vector<Sound> Sounds;
std::vector<Music> music;

void InitResourses() {

    LoadScreen = new LoadingScreen();
    SetActiveScreen(LoadScreen);

    GuiLoadStyle("resources/styles/ashes/ashesR2.rgs");
    int fontc = 78;

    fuentes.push_back(LoadFont("resources/fuentes/GREEN_NATURE.ttf"));

    TtoLoad.push_back("resources/Tilemap/tilemap.png");

    TtoLoad.push_back("resources/levels/default.png");

    TtoLoad.push_back("resources/Tilemap/watch.png");

    MtoLoad.push_back("resources/Sounds/ArcadeKind.mp3");

    StoLoad.push_back("resources/Sounds/0Fell.wav");
    StoLoad.push_back("resources/Sounds/0Die.wav");
    StoLoad.push_back("resources/Sounds/0JumpHit_01.wav");
    StoLoad.push_back("resources/Sounds/qubodupImpactStone.wav");
    StoLoad.push_back("resources/Sounds/menu.wav");

    TotalToLoad = TtoLoad.size() + StoLoad.size() + MtoLoad.size();
}

void FinalizeLoad() {

    LoadSprites(0, 36, 42, 1);

    SetCustomSpriteOrigin(540, { 8,11 });
    SetCustomSpriteOrigin(541, { 8,11 });
    SetCustomSpriteOrigin(542, { 8,11 });
    SetCustomSpriteOrigin(543, { 8,11 });
    SetCustomSpriteOrigin(544, { 8,11 });
    SetCustomSpriteOrigin(545, { 8,11 });
    SetCustomSpriteOrigin(546, { 8,11 });

}


void UnloadAll() {
    for (int i = 0; i < fuentes.size(); i++) {
        UnloadFont(fuentes[i]);
    }

    for (int i = 0; i < sprites.size(); i++) {
        UnloadTexture(sprites[i]);
    }

    for (int i = 0; i < Sounds.size(); i++) {
        UnloadSound(Sounds[i]);
    }
    for (int i = 0; i < music.size(); i++) {
        UnloadMusicStream(music[i]);
    }
}

const Texture2D& GetTexture(int id) {
    if (id < 0 || id > sprites.size()) {

        return sprites[0];
    }
    return sprites[id];
}
const Music& GetMusic(int id) {
    if (id < 0 || id > music.size()) {

        return music[0];
    }

    music[id].looping = true;

    return music[id];
}
const Sound& GetSound(int id) {
    if (id < 0 || id > Sounds.size()) {

        return Sounds[0];
    }

    return Sounds[id];
}
const Font& GetFont(int id) {
    if (id < 0 || id > fuentes.size()) {

        return fuentes[0];
    }

    return fuentes[id];
}

void UpdateLoad() {

    //esta funcion tambien esta robadita del rpg example xdd https://github.com/raylib-extras/RPGExample/blob/main/RPG/loading.cpp
    if (TtoLoad.empty() && StoLoad.empty() && MtoLoad.empty())
    {
        FinalizeLoad();
        LoadComplete();
        return;
    }

    // load some resources
    // we don't want to load them all in one shot, that may take some time, and the app will look like it is dead
    // so we only load a few per frame.
    const int maxToLoadPerFrame = 1;

    for (int i = 0; i < maxToLoadPerFrame; ++i)
    {
        if (!TtoLoad.empty())
        {
            sprites.push_back(LoadTexture(TtoLoad.front().c_str()));
            TtoLoad.pop_front();

            LoadedItems++;
        }
        else if (!MtoLoad.empty())
        {
            music.push_back(LoadMusicStream(MtoLoad.front().c_str()));
            MtoLoad.pop_front();

            LoadedItems++;
        }
        else if (!StoLoad.empty())
        {
            Sounds.push_back(LoadSound(StoLoad.front().c_str()));
            StoLoad.pop_front();

            LoadedItems++;
        }
        LoadScreen->Progress = LoadedItems / float(TotalToLoad);
    }
}