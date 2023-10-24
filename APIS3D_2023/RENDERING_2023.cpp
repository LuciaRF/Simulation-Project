#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "glrender.h"
#include "triangle3d.h"
#include "world.h"
#include "state.h"
#include "cubo3d.h"

#include <GLFW/glfw3.h>
#include "eventmanager.h"
#include "Billboard.h"
#include "particle.h"
#include "RayTrace.h"
#include "renderType.h"


/**********************
* Escenario que contiene una tetera, la cabeza de mono de cristal, y una caja detrás de la cámara
************************/
void scene1(World* world)
{
	//posición junto a la cámara
	Light* light = new Light(glm::vec3(1, 0, 2), Light::POINT);
	light->setLinearAttenuation(0.0f);
	light->setSize({ 0.4, 0.4, 0.0 });

	//posición centrada
	Camera* cam = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	
	cam->setProjection(glm::perspective(45.0, 4.0 / 3.0, 0.1, 1000.0));
	State::lights.push_back(light);
	world->setAmbient(glm::vec3(0.5, 0.5, 0.5));

	world->addCamera(cam);
	world->setActiveCamera(0);
	

	cubo3d* skybox = new cubo3d();
	skybox->loadMSH("data/skybox_2.msh");
	skybox->setSize(glm::vec3(100, 100, 100));

	cubo3d* cubo = new cubo3d();
	cubo->loadMSH("data/lightCube.msh");
	cubo->setPos(glm::vec3(0, 0, 5));

	cubo3d* teapot = new cubo3d();
	teapot->loadMSH("data/teapot_reflect.msh");
	teapot->setPos(glm::vec3(0, 0, -3));

	cubo3d* suzanne = new cubo3d();
	suzanne->loadMSH("data/suzanne_refract.msh");
	suzanne->setPos(glm::vec3(1, 0, -1));
	
	
	world->addObject(skybox);	
	world->addObject(cubo);	
	world->addObject(teapot);
	world->addObject(suzanne);
	

}

/**************
* Escenario con el skybox, y la escena de sombras con una pared, un cilindro y una bola flotando
********/
void scene2(World* world)
{
	Light* light = new Light(glm::vec3(0, 10, 6), Light::POINT);
	Camera* cam = new Camera(glm::vec3(2, 5, 10), glm::vec3(0, 1, 0), glm::vec3(0, 0, 3));
	light->setLinearAttenuation(0.0f);
	light->setSize({ 0.1, 0.1, 0.0 });

	cam->setProjection(glm::perspective(45.0, 4.0 / 3.0, 0.1, 1000.0));

	State::lights.push_back(light);
	State::ambient = glm::vec3(0.5f);
	world->setAmbient(glm::vec3(0.5, 0.5, 0.5));

	//posición junto a la cámara	
	world->addCamera(cam);
	world->setActiveCamera(0);


	cubo3d* skybox = new cubo3d();
	skybox->loadMSH("data/skybox_2.msh");
	skybox->setSize(glm::vec3(100, 100, 100));


	cubo3d* scene = new cubo3d();
	scene->loadMSH("data/scene.msh");
	scene->setRot(glm::vec3(0, 3.14f, 0));

	world->addObject(skybox);
	world->addObject(scene);

}


void scene3(World* world)
{
	//posición junto a la cámara
	Light* light = new Light(glm::vec3(0, 0.8f, 0.0f), Light::POINT);
	light->setColor(glm::vec3(0.9, 0.9, 0.8));
	light->setLinearAttenuation(1.1f);
	light->setSize({ 0.4, 0.0, 0.4 });

	//posición centrada
	Camera* cam = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	cam->setProjection(glm::perspective(45.0, 4.0 / 3.0, 0.1, 1000.0));
	State::lights.push_back(light);
	world->setAmbient(glm::vec3(0.3, 0.3, 0.3));

	world->addCamera(cam);
	world->setActiveCamera(0);


	cubo3d* cornell = new cubo3d();
	cornell->loadMSH("data/cornell_box.msh");
	cubo3d* whiteTeapot = new cubo3d();
	whiteTeapot->loadMSH("data/whiteTeapot.msh");
	whiteTeapot->setSize(glm::vec3(0.5, 0.5, 0.5));
	whiteTeapot->setPos(glm::vec3(0.0, -0.58, 0));



	world->addObject(cornell);
	world->addObject(whiteTeapot);
}


void scene4(World* world)
{
	//posición junto a la cámara
	Light* light = new Light(glm::vec3(0, 0.0f, 1.0f), Light::POINT);
	light->setColor(glm::vec3(0.9, 0.9, 0.8));
	light->setLinearAttenuation(1.1f);
	light->setSize({ 0.4, 0.4, 0.0 });
	//posición centrada
	Camera* cam = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	cam->setProjection(glm::perspective(45.0, 4.0 / 3.0, 0.1, 1000.0));
	State::lights.push_back(light);
	world->setAmbient(glm::vec3(0.3, 0.3, 0.3));

	world->addCamera(cam);
	world->setActiveCamera(0);


	cubo3d* cornell = new cubo3d();
	cornell->loadMSH("data/cornell_box.msh");
	
	cubo3d* bunny = new cubo3d();
	bunny->loadMSH("data/bunny.msh");
	bunny->setSize(glm::vec3(6.0, 6.0, 6.0));
	bunny->setRot(glm::vec3(3.1416f / 2.0f, 0, 0));
	bunny->setPos(glm::vec3(0.0, -1.15, 0));
	/*
	cubo3d* whiteTeapot = new cubo3d();
	whiteTeapot->loadMSH("data/crystalTeapot.msh");
	whiteTeapot->setSize(glm::vec3(0.5, 0.5, 0.5));
	whiteTeapot->setPos(glm::vec3(0.0, -0.59, 0));
	*/
	/*cubo3d* bunny = new cubo3d();
	bunny->loadOBJ("data/obj/bunny.obj");
	bunny->setSize(glm::vec3(0.25, 0.25, 0.25));
	bunny->setRot(glm::vec3(3.1416f / 2.0f, 0, 0));
	bunny->setPos(glm::vec3(0.0, -0.91f, 0));
	*/
	/*cubo3d* tyra = new cubo3d();
	tyra->loadOBJ("data/obj/tyra.obj");
	tyra->setPos(glm::vec3(0.0, -0.66f, 0));
	tyra->setRot(glm::vec3(0, -3.1416f / 2.0f, 0));
	tyra->setSize(glm::vec3(0.25, 0.25, 0.25));
	*/

	world->addObject(cornell);
	//world->addObject(tyra);
	world->addObject(bunny);
	//world->addObject(whiteTeapot);

}

int main(int argc, char** argv)
{

   
#if defined(RAYTRACE) 
	Render* render = new RayTrace();
#elif defined(OPENGLRENDER) || defined(CUDARENDER)	//generar id de textura
	Render* render = new GLRender();


	State::defaultShader = new GLSLProgram();
	State::defaultShader->addShader("data/program.fragment");
	State::defaultShader->addShader("data/program.vertex");
	State::defaultShader->linkProgram();
#endif
	
	//posición para la escena con sombras

    World* world=new World();

	//scene1(world);
	//scene2(world);
	scene3(world);
	//scene4(world);

	
		
	int nobj=(int)world->getNumObjects();
	for(int i=0;i<nobj;i++)
	{
		render->setupObj(world->getObject(i));
	}

	State::currentTime = State::lastTime = (float)glfwGetTime();
	float gradX = 1.0f;
	float gradY = 1.0f;
    while (!render->shouldClose())
    {
		State::currentTime = (float)glfwGetTime();
		State::deltaTime = State::currentTime - State::lastTime;
		State::lastTime = State::currentTime;
        world->update(State::deltaTime);
        render->drawWorld(world);

  
    }
    return 0;
}

