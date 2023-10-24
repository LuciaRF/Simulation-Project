#ifndef CAMERA_H
#define CAMERA_H
#include "entity.h"

class Camera : public Entity
{

     glm::mat4 projection;
     glm::vec3 clearColor;

     glm::vec3 up;
     glm::vec3 lookAt;
     glm::vec3 direction;

    

public:
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt);

    const glm::mat4& getProjection() const{return projection;}
    void setProjection(const glm::mat4& proj){projection=proj;}

    const glm::vec3& getClearColor() const{return clearColor;}
    void setClearColor(const glm::vec3& color){clearColor=color;}

    void prepare();

    void step(float timestep);
};

#endif // CAMERA_H
