#include "triangle3d.h"
#include "state.h"
#include "material.h"
triangle3D::triangle3D()
{
    ////////////////////////
    //Inicializacion/carga de mesh
    ////////////////////////

    Mesh3D* mesh=new Mesh3D();
    mesh->setColorRGB(1,0,0);

    vertex_t v;
    v.pos=glm::vec4(-0.5,-0.5,0.0,0.0f);
    mesh->addVertex(v);
    v.pos=glm::vec4( 0.5,-0.5,0.0, 0.0f);
    mesh->addVertex( v);
    v.pos=glm::vec4( 0.0,0.5,0.0, 0.0f);
    mesh->addVertex( v);

    mesh->addTriangleIdx(0,1,2);


    this->addMesh(mesh,new Material(NULL,State::defaultShader));

    this->size.x = 0.5f;
    this->size.y = 0.5f;
    this->size.z = 0.5f;

    this->rot.x = 0;
    this->rot.y = 0;
    this->rot.z = 0;

    this->pos.x = 0;
    this->pos.y = 0;
    this->pos.z = 0;
}

void triangle3D::step(float timestep)
{
    this->rot.y+=0.01f;
}
