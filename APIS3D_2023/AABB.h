#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "vertex.h"
#include "material.h"
#include "mesh3d.h"
#include <iostream>

/*
* Estructura que almacena información de un punto en el que ha colisionado un rayo
*/
typedef struct collision_t
{
	vertex_t point; //almacena posición 3D(coordenadas de cámara), color,coordenadas de textura y normal del punto en el que ha intersecado
					//no almacena la tangente
	double distance; //distancia del punto al origen del rayo
	Material* mat; //p`ropiedades del material al que pertenece este punto (transparencia, reflexión, refracción...)

}collision_t;

/*
* Estructura para simular un rayo lanzado desde un punto
*/
typedef struct collisionRay_t
{
	glm::vec3 origin; //origen del rayo
	glm::vec3 dir;  //dirección
	float distance; //distancia máxima que puede recorrer
	bool inside; //comportamiento del rayo
}collisionRay_t;

/*
* Estructura para almacenar datos de un triángulo (3 vértices)
*/
typedef struct triangle_t
{
	union {
		struct {
			vertex_t* v0; //punteros a los vértices. La estructura solo tiene referencias
			vertex_t* v1; //a vértices para poder ahorrar memoria
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
* Cada subnivel almacena una referencia (punteros) a los triángulos que le pertenecen
*/
class AABB {
public:
	//Vértices mínimos y máximos de esta caja
	glm::vec3 minB=glm::vec3(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX), maxB = glm::vec3(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);
	//Si no es una caja "hoja" (ultimos niveles) , este vector almacena una lista de las subcajas hijas
	//un máximo de 8 cajas hijas (octree)
	std::vector<AABB*> subAABBs;
	//Lista de vértices de los triángulos almacenados. Sólo se puede acceder desde la raíz
	std::vector<struct vertex_t>* vertices=nullptr;
	//lista de triángulos que están almacenados en esta caja. Si es una "hoja", sólo almacena un triángulo
	std::vector<triangle_t*> triangles;
	//lista de identificadores de vértices (la misma que vertexList en los objetos 3D)
	std::vector<glm::int32>* vIdx = nullptr;
	//copias de vértices transformados por las matrices "model view". No podemos acumular las transformaciones de las matrices, por lo que
	//en cada step, se almacena ese vértice transformado aparte
	std::vector<struct vertex_t>* transfVertices;
	//Triángulos que apuntan a los vértices multiplicados por las matrices de transformación
	//Son los que se usan en las fases de detección de colisiones
	std::vector<triangle_t*> transfTriangles;
	bool isRoot = false;
	Material* mat;
	AABB();
	/*
	* Contructor, dada una malla 3D la subdivide en varias cajas AABB para formar el octree.
	*/
	AABB(Mesh3D* mesh, Material* mat);

	/*
	* Método para actualizar la jerarquía de cajas y triángulos almacenados. En cada step, si se ha movido/cambiado de posición
	* la cámara o el objeto, se deben actualizar todas las posiciones de los triángulos y recalcular la jerarquía de cajas
	*/
	void update(glm::mat4 matrixTransf);

	/**
	* Método para crear el siguiente nivel de cajas AABB. Dada la caja actual, se crean 8 cajas hijas dividiendo por la mitad en los 
	* 4 ejes 3D. Como resultado, hay un máximo de 8 cajas hijas. En caso de no haber triángulos suficientes, se eliminan las cajas vacías
	*/
	void subdivide();
	/*
	* Método para ajustar la caja al tamaño de los triángulos que almacena
	*/
	void resize();

	/**
	* Método para calcular las colisiones entre un rayo y esta jerarquía
	* Recibe un std::map<float, collision_t> arrayColls que rellenará con las colisiones encontradas contra el rayo.
	* 
	* El mapa se encuentra ordenado en función a la distancia del origen del rayo, de más cercano a más lejano
	*/
	void  getCollisions(std::map<double, collision_t> &arrayColls, collisionRay_t ray);

	/*
	* Método para testear si un rayo colisiona con la caja del nivel actual
	*/
	bool  testCollisionAABB(collisionRay_t ray);

	/*
	* Método para testear si un rayo colisiona con un triángulo.
	*/
	static  bool triangleTestRay(triangle_t* triangle, Material* mat, collisionRay_t ray, collision_t& collision);


	/*
	* Método para testear si esta caja colisiona con un punto
	*/
	bool testCollisionPoint(glm::vec3 p);

	~AABB();
};

