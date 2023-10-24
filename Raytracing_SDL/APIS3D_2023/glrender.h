#ifndef GLRENDER_H
#define GLRENDER_H

#include "object3d.h"
#include "world.h"
#include "render.h"
//estructura que almacena los datos de una malla cargada anteriormente en openGL

typedef struct glMeshAttribs_t
{
    GLuint bufferID; //vertex buffer ID
    GLuint indexArrayID; // index buffer
    GLuint vertexArrayID; //vertex array (objeto)
} glMeshAttribs_t;

class GLRender:public Render
{

    //Lista de mallas ya cargadas en memoria de GPU. Almacena sus identificadores para
    // poder usarlos en el shader cuando se dibuje
    GLFWwindow* window = NULL;
public:
    std::map<glm::uint32, glMeshAttribs_t > vMeshIds;

    //Constructor por defecto, inicializa OpenGL y las librerías extra necesarias
    GLRender();

    //Método que realiza las operaciones necesarias para cargar la malla 3d del objeto en
    //memoria de GPU. En concreto, comprobará si no se ha cargado anteriormente sus mallas
    virtual void 	setupObj(Object3D* obj);
    //Método para dibujar por pantalla un objeto 3D. En este caso, accederá a los atributos
   //necesarios del objeto y a su shader para poder dibujarlos en pantalla
    virtual void	drawObject(Object3D* obj);

    virtual void	drawWorld(World* world);
    virtual bool shouldClose() {
        return glfwWindowShouldClose(window);
    }

};

#endif // GLRENDER_H
