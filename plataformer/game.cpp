#include <raylib.h>
#include "include/main.h"
#include "include/screens.h"

bool triggerMenu = false;

class GameScene : public Screen {
public: 
    void Draw() override {
        ClearBackground(BLACK);
        DrawRectangle(0, 0, GetScreenWidth()/2, GetScreenHeight()/2, GREEN);


    }

}gameScene;

struct platform
{

};

struct caracter
{

};

void UpdateGame() {

    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();

    SetActiveScreen(&gameScene);
}
