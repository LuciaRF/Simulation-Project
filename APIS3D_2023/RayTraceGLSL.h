#pragma once
#include "render.h"
#include "shader.h"
#include <SDL.h>


#define PI          3.14159265358979323846
#define WIDTH       640         /* Screen width */
#define HEIGHT      480        /* Screen height */

class RayTraceGLSL :
    public Render
{
    GLFWwindow* GLwindow = NULL;

    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    bool exit = false;

    GLSLProgram* program=NULL;
public:
    RayTraceGLSL();

    //M�todo que realiza las operaciones necesarias para cargar la malla 3d del objeto en
    //memoria de GPU. En concreto, comprobar� si no se ha cargado anteriormente sus mallas
    virtual void 	setupObj(Object3D* obj) override;
    //M�todo para dibujar por pantalla un objeto 3D. En este caso, acceder� a los atributos
   //necesarios del objeto y a su shader para poder dibujarlos en pantalla
    virtual void	drawObject(Object3D* obj)override;

    void dispatchEvent(SDL_Event e);

    virtual void	drawWorld(World* world)override;
    virtual bool shouldClose()override { return exit; }

    ~RayTraceGLSL();
};

