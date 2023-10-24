#ifndef CUBO3D_H
#define CUBO3D_H
#include "object3d.h"

class cubo3d: public Object3D
{
public:
    cubo3d();
    void step(float timestep);
};

#endif // CUBO3D_H
