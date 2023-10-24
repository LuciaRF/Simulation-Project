#ifndef ENTITY_H
#define ENTITY_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Entity
{
protected:
	enum Type
	{
		CAMERA,
		OBJECT3D,
		LIGHT
	};
	Type entityType;
    //Posición de nuestra entidad en coordenadas 3D
    glm::vec3 	pos;

    //Tamaño  de nuestra entidad en coordenadas 3D
    glm::vec3 	size;

    //Ángulo de rotación de nuestra entidad en cada uno de los ejes
    glm::vec3 	rot;

    //Matriz “modelo” de esta entidad.
     glm::mat4x4 modelMtx;

     int lastMX, lastMY;
public:
    Entity();

    void setPos(glm::vec3 pos){this->pos=pos;}
	glm::vec3 getPos() { return this->pos; }
    void setRot(glm::vec3 rot){this->rot=rot;}
    glm::vec3 getRot() { return this->rot; }
    void setSize(glm::vec3 size){this->size=size;}
    glm::vec3 getSize() { return this->size; }

    virtual void computeModelMtx();
    glm::mat4x4 getModelMtx();

    virtual void 	step(float deltatime)=0;

};

#endif // ENTITY_H
