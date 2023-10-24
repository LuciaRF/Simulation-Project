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
			//obtendremos un árbol binario de búsqueda de colisiones (un std::map)
			//las colisiones están ordenadas por distancia, la primera posición es la más cercana
			//al origen del rayo
			
			//Cada malla lleva asociada una jerarquía de volúmenes envolventes
			//pedimos esa jerarquía, y pedimos todas las colisiones que haya con el rayo
			ob->getAABB(meshId)->getCollisions(arrayColls, ray);
			//y guardamos la colision más cercana
			
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
