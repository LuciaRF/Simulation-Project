#pragma once
#include "Billboard.h"
class Particle :
	public Billboard
{
private:
	float spinVel=0;
	float lifetime=0;
	bool autofade = false;
	glm::vec3  vel= glm::vec3(0,0,0);

public:

	Particle(
		const char* fileName,
		const glm::vec3& vel,
		float spinVel,
		float lifetime,
		bool autofade);
	float			getRemainingLifetime() const;
	virtual void	step(float deltaTime) override;

};

