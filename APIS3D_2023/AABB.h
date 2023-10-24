#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "vertex.h"
#include "material.h"
#include "mesh3d.h"
#include <iostream>

/*
* Estructura que almacena informaci�n de un punto en el que ha colisionado un rayo
*/
typedef struct collision_t
{
	vertex_t point; //almacena posici�n 3D(coordenadas de c�mara), color,coordenadas de textura y normal del punto en el que ha intersecado
					//no almacena la tangente
	double distance; //distancia del punto al origen del rayo
	Material* mat; //p`ropiedades del material al que pertenece este punto (transparencia, reflexi�n, refracci�n...)

}collision_t;

/*
* Estructura para simular un rayo lanzado desde un punto
*/
typedef struct collisionRay_t
{
	glm::vec3 origin; //origen del rayo
	glm::vec3 dir;  //direcci�n
	float distance; //distancia m�xima que puede recorrer
	bool inside; //comportamiento del rayo
}collisionRay_t;

/*
* Estructura para almacenar datos de un tri�ngulo (3 v�rtices)
*/
typedef struct triangle_t
{
	union {
		struct {
			vertex_t* v0; //punteros a los v�rtices. La estructura solo tiene referencias
			vertex_t* v1; //a v�rtices para poder ahorrar memoria
			vertex_t* v2;
		};
		vertex_t* v[3];
	};
	
}triangle_t;


#define FLOAT_MIN -std::numeric_limits<float>::max()
#define FLOAT_MAX  std::numeric_limits<float>::max()

/*
* Clase que gestiona las colisiones entre mallas y rayos en el raytracer
* Funciona como un octree, por cada nivel se subdivide en 8 cajas alineadas con los ejes
* Cada subnivel almacena una referencia (punteros) a los tri�ngulos que le pertenecen
*/
class AABB {
public:
	//V�rtices m�nimos y m�ximos de esta caja
	glm::vec3 minB=glm::vec3(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX), maxB = glm::vec3(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);
	//Si no es una caja "hoja" (ultimos niveles) , este vector almacena una lista de las subcajas hijas
	//un m�ximo de 8 cajas hijas (octree)
	std::vector<AABB*> subAABBs;
	//Lista de v�rtices de los tri�ngulos almacenados. S�lo se puede acceder desde la ra�z
	std::vector<struct vertex_t>* vertices=nullptr;
	//lista de tri�ngulos que est�n almacenados en esta caja. Si es una "hoja", s�lo almacena un tri�ngulo
	std::vector<triangle_t*> triangles;
	//lista de identificadores de v�rtices (la misma que vertexList en los objetos 3D)
	std::vector<glm::int32>* vIdx = nullptr;
	//copias de v�rtices transformados por las matrices "model view". No podemos acumular las transformaciones de las matrices, por lo que
	//en cada step, se almacena ese v�rtice transformado aparte
	std::vector<struct vertex_t>* transfVertices;
	//Tri�ngulos que apuntan a los v�rtices multiplicados por las matrices de transformaci�n
	//Son los que se usan en las fases de detecci�n de colisiones
	std::vector<triangle_t*> transfTriangles;
	bool isRoot = false;
	Material* mat;
	AABB();
	/*
	* Contructor, dada una malla 3D la subdivide en varias cajas AABB para formar el octree.
	*/
	AABB(Mesh3D* mesh, Material* mat);

	/*
	* M�todo para actualizar la jerarqu�a de cajas y tri�ngulos almacenados. En cada step, si se ha movido/cambiado de posici�n
	* la c�mara o el objeto, se deben actualizar todas las posiciones de los tri�ngulos y recalcular la jerarqu�a de cajas
	*/
	void update(glm::mat4 matrixTransf);

	/**
	* M�todo para crear el siguiente nivel de cajas AABB. Dada la caja actual, se crean 8 cajas hijas dividiendo por la mitad en los 
	* 4 ejes 3D. Como resultado, hay un m�ximo de 8 cajas hijas. En caso de no haber tri�ngulos suficientes, se eliminan las cajas vac�as
	*/
	void subdivide();
	/*
	* M�todo para ajustar la caja al tama�o de los tri�ngulos que almacena
	*/
	void resize();

	/**
	* M�todo para calcular las colisiones entre un rayo y esta jerarqu�a
	* Recibe un std::map<float, collision_t> arrayColls que rellenar� con las colisiones encontradas contra el rayo.
	* 
	* El mapa se encuentra ordenado en funci�n a la distancia del origen del rayo, de m�s cercano a m�s lejano
	*/
	void  getCollisions(std::map<double, collision_t> &arrayColls, collisionRay_t ray);

	/*
	* M�todo para testear si un rayo colisiona con la caja del nivel actual
	*/
	bool  testCollisionAABB(collisionRay_t ray);

	/*
	* M�todo para testear si un rayo colisiona con un tri�ngulo.
	*/
	static  bool triangleTestRay(triangle_t* triangle, Material* mat, collisionRay_t ray, collision_t& collision);


	/*
	* M�todo para testear si esta caja colisiona con un punto
	*/
	bool testCollisionPoint(glm::vec3 p);

	~AABB();
};

