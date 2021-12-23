#include <raylib.h>
#include "include/main.h"
#include "include/game.h"
#include "include/screens.h"

bool triggerMenu = false;


class chile : public Entidad {
public:
    int aceleracion = 0;
    int masa = 5;
    int velocidad;
    void DibujarEntidad() override {
        //DrawRectangle(0+aceleracion,);
    }

}Chile;

class GameScene : public Screen {
public: 
    void Draw() override {
        ClearBackground(BLACK);
        DrawRectangle(0, 0, GetScreenWidth()/2, GetScreenHeight()/2, GREEN);

        Chile.DibujarEntidad();

    }

}gameScene;


void UpdateGame() {

    
    if (IsKeyPressed(KEY_ESCAPE)) PauseGame();
    

    SetActiveScreen(&gameScene);

}
