#ifndef WORLD_H
#define WORLD_H
#include "camera.h"
#include "object3d.h"
#include "light.h"
#include <list>
#include "Emiter.h"
class World
{
    std::list<Object3D*> objects;
    std::list<Camera*> cameras;	
	std::list<Emitter*> emisors;
	//std::list<Light*> lights;
    int activeCamera;
	glm::vec3 ambientLight;
public:
    World();

    void addObject(Object3D* obj){objects.push_back(obj);}
    void addCamera(Camera* cam){cameras.push_back(cam);}
	void addEmisor(Emitter* emisor) { emisors.push_back(emisor); }

    void removeObject(Object3D * obj){objects.remove(obj);}
    void removeCamera(Camera * cam){cameras.remove(cam);}
	void removeEmisor(Emitter* emisor) { emisors.remove(emisor); }
    size_t getNumObjects(){return objects.size();}
    size_t getNumCameras(){return cameras.size();}
	size_t getNumEmisors() { return emisors.size(); }


	void getCollisions(collisionRay_t ray, std::map<double, collision_t>&arrayColls);
    Object3D * getObject(size_t index){
        std::list<Object3D*>::iterator it = objects.begin();
        std::advance(it, index);
        return *it;}

	Camera* getCamera(size_t index){
		std::list<Camera*>::iterator it = cameras.begin();
		std::advance(it, index);
		return *it;
		}

	Emitter* getEmisor(size_t index) {
		std::list<Emitter*>::iterator it = emisors.begin();
		std::advance(it, index);
		return *it;
	}
	/*
	Light* getLight(size_t index) {
		std::list<Light*>::iterator it = lights.begin();
		std::advance(it, index);
		return *it;
	}*/
    void	update(float deltaTime);
    void setActiveCamera(size_t index){activeCamera=(int)index;}

    int getActiveCamera(){return activeCamera;}
	const glm::vec3& getAmbient() const {
		return ambientLight;
	}
	void setAmbient(const glm::vec3& ambient) { ambientLight = ambient; }

};

#endif // WORLD_H
