#include "entity.h"

Entity::Entity()
{

}
/*
glm::quat ToQuaternion(float yaw, float pitch, float roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);

    glm::quat q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}


void Entity::computeModelMtx() {

    glm::mat4 trans = glm::translate(glm::mat4(1.0), pos);

    glm::mat4 rot = glm::toMat4(ToQuaternion(this->rot.x, this->rot.y, this->rot.z));

    glm::mat4 scal = glm::scale(glm::mat4(1.0), this->size);
    this->modelMtx = trans * rot * scal;
}*/

void Entity::computeModelMtx(){

    glm::mat4 trans=glm::translate(glm::mat4(1.0),pos);
    glm::mat4 rot=glm::rotate(glm::mat4(1.0),this->rot.x,glm::vec3(1, 0, 0))*
                glm::rotate(glm::mat4(1.0), this->rot.y, glm::vec3(0, 1, 0))*
                glm::rotate(glm::mat4(1.0), this->rot.z, glm::vec3(0, 0, 1));
    glm::mat4 scal=glm::scale(glm::mat4(1.0),this->size);
    this->modelMtx= trans* rot*scal;
}

glm::mat4x4 Entity::getModelMtx()
{
    return this->modelMtx;
}
