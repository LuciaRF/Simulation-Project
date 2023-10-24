#include "Emiter.h"
#include <glm/gtc/random.hpp>
#include "state.h"

Emitter::Emitter(const char* mshFile, float emissionRate, bool autofade)
{
	this->mshFile = mshFile;
	this->emissionRate = emissionRate;
	
	particlesToEmitAccum = 0;
}
 void	Emitter::step(float deltaTime) 
{
	 //añadir
	 double newParticlesCount = emissionRate * deltaTime + particlesToEmitAccum;
	 int particlesToAdd = static_cast<int>(newParticlesCount) / 1;
	 particlesToEmitAccum = newParticlesCount - (double)particlesToAdd;

	 //actualizar
	 for (int i = 0; i < particlesToAdd; i++)
	 {
		 Particle* npart = new Particle(mshFile,
			 glm::linearRand(minVel,maxVel),
			 glm::linearRand(spinMin, spinMax),
			 glm::linearRand(lifeMin, lifeMax),false );
		 npart->setPos(this->getPos());
		 npart->setSize (glm::vec3(0.1, 0.1, 0.1));		 
		 this->particles.push_back(npart);
	 }

	 std::list<Particle*>::iterator i = particles.begin();

	 for (; i != particles.end(); i++)
	 {
		 (*i)->step(deltaTime);
	 }

	 //ordenar
	 std::map<float, Particle*> sorted;

	 i = particles.begin();
	 
	 while (i != particles.end())
	 {
		 float distance = State::activeCamera->getPos().z - (*i)->getPos().z;
		 if ((*i)->getRemainingLifetime() > 0)
		 {
			 sorted[distance] = (*i);
			 ++i;
		 }
		 else
		 {
			 Particle* p = (*i);	
			 i = particles.erase(i);
			 delete p;
		 }
	
	
		 
	 }	
	 particles.clear();
	 for (std::map< float, Particle* >::iterator i = sorted.begin(); i != sorted.end(); i++)
	 {
		 particles.push_back(i->second);
	 }
}