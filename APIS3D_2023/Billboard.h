#pragma once
#include "object3d.h"
#include "material.h"
class Billboard :
	public Object3D
{
protected:	
	
	float spin;
public:
	Billboard();	
	float getSpin() const { return spin; }
	void	setSpin(float spin) { this->spin = spin; }
	void computeModelMtx();
	virtual void step(float deltatime) {}
};

