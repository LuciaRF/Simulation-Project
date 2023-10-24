#pragma once
#include "entity.h"
#include "particle.h"
#include <list>
class Emitter :
	public Entity
{
private:

	const char* mshFile = NULL;
	float min = 0;
	float max = 0;
	glm::vec3 minVel;
	glm::vec3 maxVel;
	float spinMin = 0;
	float spinMax = 0;
	float scaleMin = 0;
	float scaleMax = 0;
	float lifeMin = 0;
	float lifeMax = 0;
	glm::vec4 minColor;
	glm::vec4 maxColor;

	bool enable = false;

	std::list<Particle*> particles;
	double emissionRate, particlesToEmitAccum;
public:
	Emitter(const char* mshFile, float emissionRate, bool autofade);

	void			setRateRange(float min, float max) {
		this->min = min;
		this->max = max;
	}
	void			setVelocityRange(const glm::vec3& min, const glm::vec3& max)
	{
		minVel = min;
		maxVel = max;
	}
	void			setSpinVelocityRange(float min, float max)
	{
		spinMin = min;
		spinMax = max;
	}
	void			setScaleRange(float min, float max)
	{
		scaleMin = min;
		scaleMax = max;
	}
	void			setLifetimeRange(float min, float max)
	{
		lifeMin = min;
		lifeMax = max;
	}

	void			setColorRange(const glm::vec4& min, const glm::vec4& max)
	{
		minColor = min;
		maxColor = max;
	}
	void			emit(bool enable) {
		this->enable = enable;
	}
	bool			isEmitting() { return enable; }

	std::list<Particle*>* getParticleList() { return &this->particles; }
	virtual void	step(float deltaTime) override;
	

};

