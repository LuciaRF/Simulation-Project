#pragma once
#include "object3d.h"
#include "world.h"

class Render
{
public:
    Render() {}

    //Método que realiza las operaciones necesarias para cargar la malla 3d del objeto en
    //memoria de GPU. En concreto, comprobará si no se ha cargado anteriormente sus mallas
    virtual void 	setupObj(Object3D* obj)=0x00;
    //Método para dibujar por pantalla un objeto 3D. En este caso, accederá a los atributos
   //necesarios del objeto y a su shader para poder dibujarlos en pantalla
    virtual void	drawObject(Object3D* obj) = 0x00;

    virtual void	drawWorld(World* world) = 0x00;
    virtual bool shouldClose() = 0x00;
};

