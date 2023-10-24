#include "mesh3d.h"

glm::uint32 Mesh3D::IDCounter=0;

Mesh3D::Mesh3D()
{
    meshID=IDCounter;
    IDCounter++;
    // color básico de esta malla
    colorRGB=glm::vec3(0,0,0);
    //Lista de vértices que forman la malla
    vVertList =new std::vector<struct vertex_t>();
    //Lista de índices de vértices. Tres índices seguidos formarán un triángulo
    vIdxList=new std::vector<glm::int32>();

}

//devuelve el identificador de esta malla
glm::uint32  Mesh3D::getMeshID()
{
    return meshID;
}
//Método para añadir vértices a la malla
void 	Mesh3D::addVertex(struct vertex_t vert)
{
    vVertList->push_back(vert);
}

//Método para añadir un triángulo en base a tres índices de vértices previamente
//añadidos
void Mesh3D::addTriangleIdx(glm::int32 v1, glm::int32 v2, glm::int32 v3)
{
    this->vIdxList->push_back(v1);
    this->vIdxList->push_back(v2);
    this->vIdxList->push_back(v3);
}

//Método para acceder a la lista de vértices almacenada
std::vector<struct vertex_t>*	Mesh3D::getVertList()
{
    return vVertList;
}

//Método para acceder a la lista de identificadores de vértices almacenada
std::vector<glm::int32>*  Mesh3D::getTriangleIdxList()
{
     return vIdxList;
}

Mesh3D::~Mesh3D()
{
    delete vVertList;
    delete vIdxList;
}
