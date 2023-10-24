#include "cubo3d.h"
#include "state.h"
#include "eventmanager.h"
cubo3d::cubo3d():Object3D()
{
    ////////////////////////
    //Inicializacion/carga de mesh
    ////////////////////////
	
    this->size.x = 1.0f;
    this->size.y = 1.0f;
    this->size.z = 1.0f;

    this->rot.x = 0;
    this->rot.y = 0;
    this->rot.z = 0;

    this->pos.x = 0;
    this->pos.y = 0;
    this->pos.z = 0;
}

void cubo3d::step(float timestep)
{
	if (this->objId != 0) {
		//this->rot.y += 0.5f;
		if (eventManager::keyb_State[GLFW_KEY_O])
			this->rot.y += 0.0001f;
		if (eventManager::keyb_State[GLFW_KEY_P])
			this->rot.y -= 0.0001f;

		if (eventManager::keyb_State[GLFW_KEY_U])
			this->rot.x += 0.0001f;
		if (eventManager::keyb_State[GLFW_KEY_Y])
			this->rot.x -= 0.0001f;

		if (eventManager::keyb_State[GLFW_KEY_LEFT])
			this->pos.x -= 0.0001f;
		if (eventManager::keyb_State[GLFW_KEY_RIGHT])
			this->pos.x += 0.0001f;

		if (eventManager::keyb_State[GLFW_KEY_UP])
			this->pos.z -= 0.0001f;
		if (eventManager::keyb_State[GLFW_KEY_DOWN])
			this->pos.z += 0.0001f;

		if (eventManager::keyb_State[GLFW_KEY_8])
			this->pos.y += 0.0001f;
		if (eventManager::keyb_State[GLFW_KEY_2])
			this->pos.y -= 0.0001f;
	}

}
