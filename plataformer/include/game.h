#pragma once

class Entidad {
public:
    virtual void DibujarEntidad();
    int aceleracion;
    int masa;
    Texture2D textura;

};

class plataforma {
public:
    virtual void DibujarPlataforma();
    Texture2D textura;
    float friccion;

};

void UpdateGame();

            
 