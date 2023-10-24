#include "world.h"

World::World()
{
    activeCamera=0;
	ambientLight = glm::vec3(1, 1, 1);

}

void World::getCollisions(collisionRay_t ray, std::map<double, collision_t> &arrayColls)
{
	int nobjects = this->getNumObjects();
	
	
	//por cada objeto, comprobar si choca con nuestro rayo de luz
	
	for (int objs = 0; (objs < nobjects) ; objs++)
	{
		Object3D* ob = getObject(objs);
		int nMesh = ob->getMeshCount();
		//por cada malla del objeto
		for (int meshId = 0; (meshId < nMesh) ; meshId++)
		{
			//obtendremos un �rbol binario de b�squeda de colisiones (un std::map)
			//las colisiones est�n ordenadas por distancia, la primera posici�n es la m�s cercana
			//al origen del rayo
			
			//Cada malla lleva asociada una jerarqu�a de vol�menes envolventes
			//pedimos esa jerarqu�a, y pedimos todas las colisiones que haya con el rayo
			ob->getAABB(meshId)->getCollisions(arrayColls, ray);
			//y guardamos la colision m�s cercana
			
		}

	}
}

 void	World::update(float deltaTime)
 {
     for(std::list<Camera*>::iterator it=cameras.begin();
         it!= cameras.end();
         ++it)
     {
        (*it)->step(deltaTime);
     }

     for(std::list<Object3D*>::iterator it=objects.begin();
         it!= objects.end();
         ++it)
     {
        (*it)->step(deltaTime);
     }

	 for (std::list<Emitter*>::iterator it = emisors.begin();
		 it != emisors.end();
		 ++it)
	 {
		 (*it)->step(deltaTime);
	 }
 }
