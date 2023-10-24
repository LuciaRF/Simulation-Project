#define _CRT_SECURE_NO_WARNINGS
#include "RayTrace.h"
#include <iostream>
#include "state.h"
#include <iostream>
#include "eventmanager.h"
#include <glm/gtx/vector_angle.hpp >
#include <fstream>
#include <omp.h>


int toRGB888(glm::vec4 colorIn)
{
	int rgbColor = 0;
	//variable en formato RGBA 32bit (4 bytes)
	glm::u8vec4 color = glm::u8vec4(0xFF, 0xFF, 0xFF, 0xFF);

	color = glm::u8vec4((unsigned char)(colorIn.r * 255.0f), (unsigned char)(colorIn.g * 255.0f),
		(unsigned char)(colorIn.b * 255.0f), (unsigned char)(colorIn.a * 255.0f));

	//y retornar el pixel resultado
	rgbColor = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
	return rgbColor;
}
RayTrace::RayTrace()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
		return;
	}
	window = SDL_CreateWindow(
		"RayTracer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (window == NULL) {
		fprintf(stderr, "could not create window: %s\n", SDL_GetError());
		return;
	}


	screenSurface = SDL_GetWindowSurface(window);
	std::cout << SDL_GetPixelFormatName(screenSurface->format->format) << "\n";
	std::cout << screenSurface->w << "\n";
	std::cout << screenSurface->h << "\n";
	std::cout << screenSurface->pitch << "\n";
}

void RayTrace::setupObj(Object3D* obj)
{
}

void RayTrace::drawObject(Object3D* obj)
{

}
void RayTrace::dispatchEvent(SDL_Event e) {
	switch (e.type) {

	case SDL_KEYDOWN:
		eventManager::keyb_State['A' + e.key.keysym.sym - 'a'] = 1;
		switch (e.key.keysym.sym) {
		case SDLK_ESCAPE:
			exit = true;
			break;

		default:
			break;
		}
		break;
	case SDL_KEYUP:
		eventManager::keyb_State[e.key.keysym.sym - 'a'] = 0;
		break;
	case SDL_WINDOWEVENT:

		switch (e.window.event) {

		case SDL_WINDOWEVENT_CLOSE:
			exit = true;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}
/**
* Método que calcula un vector aleatorio dentro de un ángulo dado respecto de un vector base
*/
glm::vec3 getRandRay(glm::vec3 norm, float angle)
{
	glm::vec3 randRay = glm::normalize(glm::vec3(((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
		((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
		((float)(std::rand() % 2000) / 1000.0f) - 1.0f));

	while (glm::angle(norm, randRay) > angle)
	{
		randRay = glm::normalize(glm::vec3(((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
			((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
			((float)(std::rand() % 2000) / 1000.0f) - 1.0f));
	}
	return randRay;
}

/**
* Método que calcula un vector aleatorio dentro entre dos ángulos "mínimo y máximo" respecto de un vector base
*/

glm::vec3 getRandRayAngles(glm::vec3 norm, float angleMin, float angleMax)
{
	glm::vec3 randRay = glm::normalize(glm::vec3(((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
		((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
		((float)(std::rand() % 2000) / 1000.0f) - 1.0f));

	while (glm::angle(norm, randRay) < angleMin || glm::angle(norm, randRay) > angleMax)
	{
		randRay = glm::normalize(glm::vec3(((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
			((float)(std::rand() % 2000) / 1000.0f) - 1.0f,
			((float)(std::rand() % 2000) / 1000.0f) - 1.0f));
	}
	return randRay;
}

/**
* Método para averiguar si un punto es visible desde otro punto. Lanza un rayo desde el punto P1 al punto P2, y si hay un objeto intermedio , devuelve "false" independientemente
* si ese objeto es transparente
*/
bool isVisible(glm::vec4 p1, glm::vec3 normal, glm::vec4 p2, World* world, collision_t& coll)
{
	bool visible = true;
	glm::vec3 dir = p2 - p1;
	dir = glm::normalize(dir);
	collisionRay_t ray;
	ray.dir = dir;
	ray.origin = p1 + 0.001f * glm::vec4(normal, 0.0f);
	ray.inside = false;
	std::map<double, collision_t> arrayCollsNext;

	float distanceLight = glm::length(p2 - p1);

	world->getCollisions(ray, arrayCollsNext);
	auto it = arrayCollsNext.begin();
	if (it != arrayCollsNext.end())
	{
		coll = it->second;
		float distance = glm::length(coll.point.pos - p1);
		//Ha chocado y está entre el objeto y la luz
		//se deja en negro/iluminación ambiental/porcentaje de oscuridad
		if ((distance < distanceLight))
			visible = false;
	}
	return visible;
}

/**
* Cálculo de contribución de luzen un punto "coll" respecto de un rayo de incidencia
*/

glm::vec3 computeLight(collisionRay_t rayIn, collision_t coll, Light* light, Material* mat)
{
	glm::vec3 finalLight = glm::vec3(1.0, 1.0, 1.0);
	//iluminación ambiental, difusa y especular (como  en el shader)
	//La posición de la luz se puede obtener del objeto "light" (color y posición)

	glm::vec3 posicionLuz = State::viewMatrix * glm::vec4(light->getPos(), 1.0f);
	//La posición del ojo se corresponde con el origen del rayo de incidencia (la luz especular deslumbrará
	//al el origen del rayo)
	glm::vec3 posicionCamara = rayIn.origin;
	//Las propiedades del punto a iluminar (posición, normal, etc...) se obtiene del objeto collision

	glm::vec3 posicionPuntoIluminar = coll.point.pos;
	glm::vec3 posicionPuntoNormal = coll.point.normal;
	

	//Las propiedades del objeto (shinniness) se pueden obtener del material
	// calcular el material mat->getShininess()
	int brillomaterial = coll.mat->getShininess();

	//Luz ambiente: se obtiene de  State::ambient

	glm::vec3 luzambiente = State::ambient;


	// Calcular la dirección de la luz incidente
	glm::vec3 lightDir = glm::normalize(posicionLuz - posicionPuntoIluminar);

	// Calcular la dirección de la vista (ojo) hacia el punto
	glm::vec3 viewDir = glm::normalize(posicionCamara - posicionPuntoIluminar);

	// Calcular la dirección de reflexión de la luz
	glm::vec3 reflectDir = glm::reflect(-lightDir, posicionPuntoNormal);


	//luz difusa:  max(dot(norm, lightDir), 0.0);
	
	float luzdifusa = glm::max(glm::dot(posicionPuntoNormal, lightDir), 0.0f);
	glm::vec3 diffuseLight = luzdifusa * light->getColor();
	
	//luz especular:  pow(max(dot(viewDir, reflectDir), 0.0), shinniness);

	glm::vec3 specularLight = glm::vec3(0, 0, 0);
	float luzespecular = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), brillomaterial);

	specularLight = luzespecular * light->getColor();


	//finalLight = luzambiente + (diffuseLight + specularLight);
	//opcional: añadir atenuación lineal: Dividir por la distancia al cuadrado

	float distance = glm::distance(posicionPuntoIluminar, posicionLuz);
	float attenuation = 1.0f / (distance/2 * distance);

	if (light->getLinearAttenuation() > 1.0f) 
	{
		finalLight = luzambiente + attenuation * (diffuseLight + specularLight);   
	}
	else
	{
		finalLight = luzambiente + diffuseLight + specularLight;
		//finalLight = luzambiente + attenuation * (diffuseLight + specularLight);
		
	}
	return finalLight;
}


glm::vec4 RayTrace::computeColorLight(World* world, int objID, collision_t coll, std::vector<Light* > lights, collisionRay_t rayIn, int bounces, bool firstBounce)
{
	//variable color en formato float
	glm::vec4 finalColor;


	//sacar el color del punto de colisión, y guardarlo en el color final en formato 3 float

	//si el material tiene textura
	if (coll.mat->getTexture() != NULL && coll.mat->getTexture()->getText2DBytes() != NULL) {

		//averiguar el tamaño de la textura (coll.mat->getTexture->getSize)
		glm::ivec2 textSize = coll.mat->getTexture()->getSize();
		//averiguar las coordenadas de textura en el que hubo una colisión (coll.point.texCoords). 
		glm::vec2 textCoords = coll.point.texCoords;
		//Estas coordenadas están en "porcentaje de x,y", quiere decir que hay que pasarlas a una posición 2D.
		//Para ello, se multiplica por el tamaño de la textura en X,Y
	//conseguir el color de la textura en las coordenadas calculadas anteriormente (getTexture->getColorAtXY())
		finalColor = coll.mat->getTexture()->getColorAtXY((textSize.x - 1) * textCoords.x, (textSize.y - 1) * textCoords.y);
	}else//si no tiene textura, 
		//es el color en el punto de colisión (coll.point.color)
	finalColor = coll.point.color;
	//.....TO DO.....//

	//calcular iluminacion (PHONG,GORAUD, LAMBERT.....)
	// TO DO //
	//OJO, calcular las coordenadas de la luz también en coordenadas de la cámara
	//obtener una luz del array de luces
	// 
//for... lights

	Light* light = nullptr;
	//acumulador de luz.
	glm::vec3 finalLight = glm::vec3(1, 1, 1);
	light = lights[0];

	if (coll.mat->getLighting())//si el objeto se puede sombrear/iluminar
	{
		//se calcula su luz
		finalLight = computeLight(rayIn, coll, light, coll.mat);
	}

	glm::vec4 visibility = glm::vec4(1.0f);

	if (coll.mat->getReceiveShadows() && !rayIn.inside) { //receive shadows

		//OPCION 1:		
		//Calcular sombras "duras"
		// Lanzar un rayo desde este punto a la luz, y si no colisiona con objetos
		// es que se puede iluminar


		//glm::vec4 pointPos = coll.point.pos;
		//glm::vec3 lightPos = light->getPos();

		//glm::mat4 matCamera = State::viewMatrix;

		//glm::vec4 lightPosition = matCamera * glm::vec4(lightPos, 1.0f);

		//collision_t collisionObject;

		//INICIO  codigo

		//float distanceLight2 = glm::length(lightPosition - pointPos);

		//bool visible;

		//visible = isVisible(coll.point.pos, coll.point.normal, lightPosition,world, collisionObject);

		//if (collisionObject.distance < distanceLight2) {
		//	
		//	visibility = glm::vec4(0.0f);

		//}
		//else {
		//	visibility = glm::vec4(1.0f);
		//}

		//OPCION 2:
		//Soft shadows:
		//Mover ligeramente la luz N veces, y comprobar si es visible
		//La media del número de rayos visibles será el porcentage de visibilidad

		//opcion 1

		//collision_t collision;
	/*	bool visible = isVisible(coll.point.pos, coll.point.normal, lightPosition, world, collision);
		float prodEscalar = 0.0f;
		float cos = 0.0f;

		if (!visible)
		{
			glm::vec3 lightDir = lightPosition - coll.point.pos;
			lightDir = glm::normalize(lightDir);
			prodEscalar = glm::dot(coll.point.normal, lightDir);
			cos = prodEscalar / (glm::length(coll.point.normal) * glm::length(lightDir));
			visibility = glm::vec4(1.0f - glm::abs(prodEscalar));
		}*/


		//soft opcion 2

		glm::vec4 pointPos = coll.point.pos;
		glm::mat4 matCamera = State::viewMatrix;
		glm::vec4 lightPos = matCamera * glm::vec4(light->getPos(), 1.0f);

		collision_t collisionObject;
		//INICIO  codigo

		bool visible;
		float count = 0;
		float total = 0;
		float step = 0.5f;//distancia total en la que movemos la luz
		//float subDivStep = step / 1.0f; //resolución de 2 puntos por eje,8 en total, ignoramos el centro 
		float subDivStep = step / 2.0f; //resolución de 3 puntos por eje,27 en total aquí sí tiene en cuenta el centro, más lento 

		for (float offsetZ = -step / 2.0f; offsetZ < ((step + 0.001f) / 2.0f); offsetZ += subDivStep)
			for (float offsetY = -step / 2.0f; offsetY < ((step + 0.001f) / 2.0f); offsetY += subDivStep)
				for (float offsetX = -step / 2.0f; offsetX < ((step + 0.001f) / 2.0f); offsetX += subDivStep) {
					glm::vec4 lightPosition = lightPos + glm::vec4(offsetX, offsetY, offsetZ, 0.0f);
					visible = isVisible(coll.point.pos, coll.point.normal, lightPosition, world, collisionObject);
					if (visible) {
						count += 1.0f;
					}
					total += 1.0f;
				}

		visibility = glm::vec4(count / total);



		//opcion 3 soft shadows
		//mover los puntos de luz alrededor de un cubo de dimensión 8

		//int numPoints = 10;
		//float distNormal = 0.03;

		//bool visible;

		//visible = isVisible(coll.point.pos, coll.point.normal, lightPosition,world, collisionObject);

		/*collision_t collision;
		glm::vec4 posMovimiento = glm::vec4(1.0f);

		float contLuces = 0.0f;
		int contador = 0;

		for (int i = 0; i < numPoints; i++) {

			posMovimiento = coll.point.pos + ((float) i * distNormal * glm::vec4(coll.point.normal, 1.0f));

			visible = isVisible(posMovimiento, coll.point.normal, lightPosition, world, collision);
			if (visible) {
				contador++;
			}

		}
		contLuces = (float)contador / (float)numPoints;
		visibility =visibility * contLuces;*/

	}
	finalColor *= glm::vec4(finalLight, 1.0f) * visibility;

	//CALCULAR ILUMINACION POR REBOTES
	// 
	//dado que final light había sido oscurecida por la sombra, se multiplica con la suma de componentes de luz
			//iluminación global (rayos incidiendo que contribuyen a la iluminación actual
			// crear entre 5 y 10 rayos aleatorios
			// comprobar si colisionan, y recoger los datos de la colisión
			//		//color/iluminación de ese punto
			//      //Dependiendo del tipo de material (shinniness) reflejarán más o menos luz
			//      //la luz se difumina según el cuadrado de la distancia
			// 
			//	contribución difusa: sumarlos todos, y dividir entre el número de rayos lanzados
			//	aplicar la iluminación al color

		//calcular reflejos

	if (coll.mat->getReflection())
	{
		glm::vec4 bouncedColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//podemos usar "número de rebotes máximo", el máximo de unidades que puede recorrer el rayo
		//o ambas
		if (bounces > 0)
		{
			//si hay rebotes o camino por recorrer
			//crear un rayo con la dirección reflejada (glm::reflect)

			collisionRay_t rayoReflejo = collisionRay_t();
			rayoReflejo.dir = glm::reflect(rayIn.dir, coll.point.normal);			

			//asignar el origen del rayo (moverlo para que no colisione con sigo mismo)
			rayoReflejo.origin = coll.point.pos + 0.01f * glm::vec4(rayoReflejo.dir, 1.0f);
			//lanzar el rayo, y conseguir la contribución de color reflejado (usar traceRay)
			rayoReflejo.inside = rayIn.inside;


			//agregarlo al color actual, en función de "cómo de reflectante es" (usar shinniness)

			glm::vec4 bouncedColor = traceRay(world, rayoReflejo, bounces - 1, false);

			float alpha = coll.mat->getShininess() / 255.0f;
			//finalColor = "porcentaje de color original"+ "porcentaje de color reflejado";

			finalColor = (1 - alpha) * finalColor + alpha * bouncedColor;
		}
	}

	//Calcular la refracción, igualmente, dependerá del número de rebotes sobrante y de la distancia recorrida
	if (coll.mat->getRefraction())
	{
		glm::vec4 bouncedColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//si hay rebotes o distancia
		if (bounces > 0)
		{
			//la normal tiene que ser la del objeto o la contraria a la normal de objeto para hacer la refraccion de entrada o de salida
			glm::vec3 normal = rayIn.inside ? -coll.point.normal : coll.point.normal;

			//crear un nuevo rayo

			collisionRay_t rayoRefraccion = collisionRay_t();

			//ojo, no es lo mismo estar dentro del objeto que fuera. La refracción depende de la dirección de la normal, por lo que si se está dentro del objeto,
			//el rayo refractado deberá calcularse con la normal invertida. Usar glm::refract

			rayoRefraccion.dir = glm::refract(rayIn.dir, normal, coll.mat->getRefractionCoef());

			//asignar el origen del rayo en el punto actual, movido a lo largo de la dirección del rayo refractado
			rayoRefraccion.origin = coll.point.pos + 0.01f * glm::vec4(rayoRefraccion.dir, 1.0f);
			rayoRefraccion.inside = !rayIn.inside;

			//trazar el rayo para conseguir el resultado (traceRAy)

			bouncedColor = traceRay(world, rayoRefraccion, bounces-1, false);

			//ecuación de fresnel mezcla de color (90% color refractado +10% color original
			
			float alpha = 0.9f;
			finalColor = (1 - alpha) * finalColor + alpha * bouncedColor;

			//si es el primer rebote, además se calcula la contribución especular (brillos en cristal)

			if (firstBounce) {

				glm::vec3 posicionPuntoIluminar = coll.point.pos;
				glm::vec3 posicionPuntoNormal = coll.point.normal;

				glm::vec3 lightPos = State::viewMatrix* glm::vec4(light->getPos(), 1.0f);

				glm::vec3 posicionCamara = rayIn.origin;

				glm::vec3 viewDir = glm::normalize(posicionCamara - posicionPuntoIluminar);

				//Las propiedades del objeto (shinniness) se pueden obtener del material
				// calcular el material mat->getShininess()
				int brillomaterial = coll.mat->getShininess();

				// Calcular la dirección de la luz incidente
				glm::vec3 lightDir = glm::normalize(lightPos - posicionPuntoIluminar);

				// Calcular la dirección de reflexión de la luz
				glm::vec3 reflectDir = glm::reflect(-lightDir, normal);

				float luzespecular = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), brillomaterial);

				finalColor += luzespecular;
			}
		}
	}

	//el color final es igual al color del objeto por la luz por la cantidad de visibilidad

	//asegurar que los colores están entre 0 y 1

	finalColor = glm::clamp(finalColor, 0.0f, 1.0f);
	//retornar color

	return finalColor;
}
/*
*
* Función encargada de detectar rayos que colisionan contra los objetos. Por cada colisión encontrada, calcula
* la iluminación/color en ese punto y devuelve el resultado
*/
glm::vec4 RayTrace::traceRay(World* world, collisionRay_t ray, int bounces, bool firstBounce)
{
	int nobjects = (int)world->getNumObjects();
	glm::vec4 colorValue(0, 0, 0, 0);//color de fondo blanco "clearColor" 
	//por cada objeto, y mientras no haya encontrado colisiones
	std::map<double, collision_t> arrayCollsFinal;

	world->getCollisions(ray, arrayCollsFinal);
	//si hay alguna colisión, nos quedamos con las de la malla más cercana
	if (arrayCollsFinal.size() > 0)
	{
		//la primera posición es la más cercana
		auto it = arrayCollsFinal.begin();
		collision_t coll = it->second;
		//podemos calcular el color e iluminación en ese punto
		colorValue = computeColorLight(world, 0, coll, State::lights, ray, bounces, firstBounce);

	}
	return colorValue;
}
/*
* Método que dibuja la escena. Recibe el mundo y sus entidades, por cada pixel de la imagen, lanza un rayo que puede intersecar
* un objeto del mundo. Si interseca, calcula el color e iluminación en ese punto
*
* La imagen 2d final se muesta en una ventana al usuario.
*/
void RayTrace::drawWorld(World* world)
{
	//Variables para eventos de teclado 
	SDL_Event e;
	SDL_PollEvent(&e);
	dispatchEvent(e);
	//iniciamos el renderizado, nadie más accede a la ventana
	SDL_LockSurface(screenSurface);
	//activamos la cámara, iluminación, etc...
	State::activeCamera = world->getCamera(world->getActiveCamera());
	State::ambient = world->getAmbient();
	State::activeCamera->computeModelMtx();
	State::activeCamera->prepare();
	//calculamos objetos
	int nobjects = (int)world->getNumObjects();
	//Array de píxels en formato ARGB 32bit. 
	int* pixels = (int*)screenSurface->pixels;
	//seteamos la imagen a blanco
	//memset(pixels, 0xFF, (int)(screenSurface->h * screenSurface->pitch));
	//actualizamos las jerarquías de detección de colisiones
	glm::mat4 model;
	for (int objs = 0; objs < nobjects; objs++)
	{
		Object3D* ob = world->getObject(objs);
		ob->computeModelMtx();
		model = ob->getModelMtx();
		int nMesh = ob->getMeshCount();
		for (int meshId = 0; meshId < nMesh; meshId++)
		{
			//actualizar transformando el modelo por sus matrices Mode y View
			ob->updateAABB(State::viewMatrix * model, meshId);
		}
	}

	//Datos de la imagen a generar
	int width = screenSurface->w;
	int height = screenSurface->h;
	float scale = (float)tan(glm::radians(51.52f * 0.5f));
	float imageAspectRatio = width / (float)height;
	//por cada pixel de la imagen
	int counter = 0;
	int* scanLines = new int[height];

	memset(scanLines, 0, sizeof(int) * height);
	//loadData(scanLines, width, height, pixels);
	SDL_UnlockSurface(screenSurface);
	SDL_UpdateWindowSurface(window);
	SDL_LockSurface(screenSurface);

	int bounces = 4;

#pragma omp parallel for num_threads(8) schedule(dynamic,8)
	for (int i = height - 1; i >= 0; i--)
		//for (int i = 474 ; i < 475; i++)
	{
		//if (!scanLines[i])
		{
			//for (int j = 85; j < 86; j++)
			for (int j = 0; j < width; j++)
			{

				//calcular un nuevo rayo desde la cámara
				float x = (2 * (j + 0.5f) / (float)width - 1) * imageAspectRatio * scale;
				float y = (1 - 2 * (i + 0.5f) / (float)height) * scale;
				glm::vec4 dir = glm::vec4(x, y, -1.0f, 0);
				//colocarlo en coordenadas de la cámara
				dir = dir;
				dir = glm::normalize(dir);
				glm::vec4 orig = State::viewMatrix * glm::vec4(State::activeCamera->getPos(), 1.0);
				//creamos el rayo con los datos
				collisionRay_t ray;
				ray.dir = dir;
				ray.origin = orig;
				ray.inside = false;
				//y lanzamos el rayo
				pixels[i * screenSurface->w + j] = toRGB888(traceRay(world, ray, bounces, true));
			}
			scanLines[i] = 1;
			if (omp_get_thread_num() == 0)
			{
				SDL_UnlockSurface(screenSurface);
				SDL_UpdateWindowSurface(window);
				// Save result to a PPM image (keep these flags if you compile under Windows)
				saveData(scanLines, width, height, pixels);
				SDL_LockSurface(screenSurface);
			}
		}
	}

	SDL_UnlockSurface(screenSurface);
	SDL_UpdateWindowSurface(window);
	saveData(scanLines, width, height, pixels);
	std::cout << "Frame\n";
}


void RayTrace::saveData(int* scanLines, int width, int height, int* pixels)
{

	std::ofstream ofsIdx("./idx.dat", std::ios::out | std::ios::binary);
	for (int l = 0; l < height; l++)
	{
		ofsIdx << scanLines[l];
	}
	ofsIdx.close();
	std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
	for (uint32_t i = 0; i < HEIGHT * WIDTH; ++i) {
		char r = (char)((glm::u8vec4*)(pixels))[i].z;
		char g = (char)((glm::u8vec4*)(pixels))[i].y;
		char b = (char)((glm::u8vec4*)(pixels))[i].x;
		ofs << r << g << b;
	}
	ofs.close();
}


void RayTrace::loadData(int* scanLines, int width, int height, int* pixels)
{

	FILE* f1 = fopen("./idx.dat", "rb");
	FILE* f2 = fopen("./out.ppm", "rb");

	if (f1 && f2)
	{
		std::string header;
		std::string oldWidth, oldHeight;
		std::string format;
		char c = 0;
		while ((c = getc(f2)) != '\n')
			header = header + c;
		while ((c = getc(f2)) != ' ')
			oldWidth = oldWidth + c;
		while ((c = getc(f2)) != '\n')
			oldHeight = oldHeight + c;
		while ((c = getc(f2)) != '\n')
			format = format + c;
		if ((width != atoi(oldWidth.c_str())) || (height != atoi(oldHeight.c_str())))
			return;

		for (int l = 0; l < height; l++)
		{
			char i = getc(f1);
			if (i == '1')
				scanLines[l] = 1;

		}
		fclose(f1);

		char* data = new char[height * width * 3];

		fread(data, 1, height * width * 3, f2);

		std::cout << "opening previous file: " << header << " " << oldWidth << " " << oldHeight << " " << format << "\n";
		for (uint32_t i = 0; i < height * width; i++) {
			((glm::u8vec4*)(pixels))[i].z = data[i * 3];
			((glm::u8vec4*)(pixels))[i].y = data[i * 3 + 1];
			((glm::u8vec4*)(pixels))[i].x = data[i * 3 + 2];
			((glm::u8vec4*)(pixels))[i].w = 0xff;
		}
		fclose(f2);
		delete[] data;
	}
}
RayTrace::~RayTrace()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}