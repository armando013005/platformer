#include "include/main.h"
#include <raylib.h>
#include "include/loading.h"
#include "include/raygui.h"
#include <vector>

std::vector<Font> fuentes;
std::vector<Texture2D> sprites;
std::vector<Texture2D> background;

void UpdateLoad() {

    GuiLoadStyle("styles/ashes/ashes.rgs");
    Font ashes = LoadFontEx("fuentes/ v5loxicarrobusta.ttf", 32, 0, 250);
    fuentes.push_back(ashes);

    sprites.push_back(LoadTexture("Tiles/Transparent/tile_0300.png"));

    background.push_back(LoadTexture("levels/default.png"));

    UpdateStartup();
}

void UnloadAll() {
    for (int i = 0; i > fuentes.size(); i++) {
        UnloadFont(fuentes[i]);
    }

    for (int i = 0; i > sprites.size(); i++) {
        UnloadTexture(sprites[i]);
    }

    for (int i = 0; i > background.size(); i++) {
        UnloadTexture(background[i]);
    }
}

const Texture2D& GetTexture(int id) {
    if (id < 0 || id > sprites.size()) {

        return sprites[0];
    }

    return sprites[id];
}

const Font& GetFont(int id) {
    if (id < 0 || id > fuentes.size()) {

        return fuentes[0];
    }

    return fuentes[id];
}

const Texture2D& GetBackground(int id) {
    if (id < 0 || id > background.size()) {

        return background[0];
    }

    return background[id];
}