#ifndef MESH3D_H
#define MESH3D_H
#include "vertex.h"
#include <vector>
class Mesh3D
{

    static glm::uint32 IDCounter;
    // Identificador único de la malla, asignado en el constructor del objeto
    glm::uint32 			meshID;
    // color básico de esta malla
    glm::vec3				colorRGB;
    //Lista de vértices que forman la malla
    std::vector<struct vertex_t>*	 vVertList ;
    //Lista de índices de vértices. Tres índices seguidos formarán un triángulo
    std::vector<glm::int32>* 	vIdxList;

public:
    //constructor por defecto de la clase. Inicializa los atributos a valores por defecto
    Mesh3D();
    ~Mesh3D();


    //devuelve el identificador de esta malla
    glm::uint32 		getMeshID();

    //Método para añadir vértices a la malla
    void			addVertex(struct vertex_t);

    //Método para añadir un triángulo en base a tres índices de vértices previamente
    //añadidos
    void			addTriangleIdx(glm::int32 v1, glm::int32 v2, glm::int32 v3);

    //Método para acceder a la lista de vértices almacenada
    std::vector<struct vertex_t>*	getVertList();

    //Método para acceder a la lista de identificadores de vértices almacenada
    std::vector<glm::int32>* 		getTriangleIdxList();

    glm::vec3 getColorRGB(){return colorRGB;}
    void setColorRGB(float r,float g, float b){
          colorRGB=glm::vec3(r,g,b);
                                                     }

};

#endif // MESH3D_H
