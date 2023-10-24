#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H
#include "object3d.h"

class triangle3D : public Object3D
{
public:
    triangle3D();
    virtual void step(float timestep);
};

#endif // TRIANGLE3D_H
