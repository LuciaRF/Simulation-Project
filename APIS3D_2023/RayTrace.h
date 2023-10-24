#pragma once
#include "render.h"
#include <SDL.h>
//1920
//1080

#define PI          3.14159265358979323846
//#define WIDTH       320          /* Screen width */
//#define HEIGHT      240        /* Screen height */

#define WIDTH       640          /* Screen width */
#define HEIGHT      480        /* Screen height */
//#define WIDTH 1920
//#define HEIGHT 1080

class RayTrace :
    public Render
{
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    bool exit = false;
public:
    RayTrace();

    //Método que realiza las operaciones necesarias para cargar la malla 3d del objeto en
    //memoria de GPU. En concreto, comprobará si no se ha cargado anteriormente sus mallas
    virtual void 	setupObj(Object3D* obj);
    //Método para dibujar por pantalla un objeto 3D. En este caso, accederá a los atributos
   //necesarios del objeto y a su shader para poder dibujarlos en pantalla
    virtual void	drawObject(Object3D* obj);

    void dispatchEvent(SDL_Event e);

    virtual void	drawWorld(World* world);
    virtual bool shouldClose() { return exit; }
    glm::vec4 computeColorLight(World* world, int objID, collision_t coll, std::vector<Light* > lights, collisionRay_t rayIn, int bounces, bool firstBounce);


    glm::vec4 traceRay(World* world, collisionRay_t ray, int bounces, bool firstBounce);


    void saveData(int* scanLines, int width, int height, int* pixels);

    void loadData(int* scanLines, int width, int height, int* pixels);

    ~RayTrace();
};

