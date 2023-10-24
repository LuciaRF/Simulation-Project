#include "particle.h"

Particle::Particle(
	const char* fileName,
	const glm::vec3& vel,
	float spinVel,
	float lifetime,
	bool autofade)
{
	this->loadMSH(fileName);
	this->autofade = autofade;
	this->lifetime = lifetime;
	this->spin = spinVel;
	this->vel = vel;
}
float			Particle::getRemainingLifetime() const
{
	return lifetime;
}

void	Particle::step(float deltaTime) 
{
	this->pos += (vel) * (float)deltaTime;
	this->lifetime -= deltaTime;
	this->rot.z += this->getSpin();
	this->size += (glm::abs(this->vel)) * (float)deltaTime;
}
