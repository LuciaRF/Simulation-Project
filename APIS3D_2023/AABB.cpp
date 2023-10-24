#include "AABB.h"
#include <utility>

 AABB::AABB()
{
	minB = glm::vec3(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
	maxB = glm::vec3(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);
	transfVertices = nullptr;
}

 AABB::AABB(Mesh3D* mesh, Material* mat)
{
	 this->mat = mat;
	isRoot = true;
	vertices = mesh->getVertList();
	transfVertices = new std::vector<struct vertex_t>();

	vIdx = mesh->getTriangleIdxList();
	//lista de triangulos originales (sin transformaciones)
	//guardamos una copia para transformaciones
	for (auto i = vertices->begin(); i != vertices->end();i++)
	{
		transfVertices->push_back((*i));
	}
	for (auto i = vIdx->begin(); i != vIdx->end();)
	{
		triangle_t* tr = new triangle_t;
		tr->v0 = &(vertices->data()[*i]);
		i++;

		tr->v1 = &(vertices->data()[*i]);
		i++;
		
		tr->v2 = &(vertices->data()[*i]);
		i++;
		triangles.push_back(tr);
	}
	//lista de triángulos multiplicados por matrices de transformacion		
	for (auto i = vIdx->begin(); i != vIdx->end();)
	{
		triangle_t* tr = new triangle_t;
		tr->v0 = &(transfVertices->data()[*i]);
		i++;
		tr->v1 = &(transfVertices->data()[*i]);
		i++;
		tr->v2 = &(transfVertices->data()[*i]);
		i++;
		transfTriangles.push_back(tr);
	}
	resize();
	subdivide();
}

 void AABB::update(glm::mat4 matrixTransf)
{
	//borrar jerarquia
	for (auto i = subAABBs.begin(); i != subAABBs.end(); i++)
	{
		delete (*i);
	}
	subAABBs.resize(0);
#pragma omp parallel for num_threads(4)
	for (int i = 0; i< vIdx->size();i++)
	{
		(*transfVertices)[(*vIdx)[i]].pos = matrixTransf * vertices->data()[(*vIdx)[i]].pos;
		(*transfVertices)[(*vIdx)[i]].normal = glm::inverse(glm::transpose(matrixTransf)) * glm::vec4(vertices->data()[(*vIdx)[i]].normal,1.0);
	}
	resize();
	subdivide();

}

inline void AABB::subdivide()
{
	if (transfTriangles.size() > 1)
	{
		AABB* subAABB[8];
		memset(subAABB, 0, sizeof(AABB*) * 8);
		int cont = 0;
		glm::vec3 step = glm::vec3((maxB.x - minB.x) / 2.0f, (maxB.y - minB.y) / 2.0f, (maxB.z - minB.z) / 2.0f);

		glm::vec3 margin = step / 2.0f;
		if (step.x == 0) step.x = 0.0001f;
		if (step.y == 0) step.y = 0.0001f;
		if (step.z == 0) step.z = 0.0001f;
		for (int i = 0; i<8; i++)
		{
			subAABB[i] = new AABB();
			subAABB[i]->mat = mat;
		}
		//dlb
		subAABB[0]->minB.x = minB.x;
		subAABB[0]->minB.y = minB.y;
		subAABB[0]->minB.z = minB.z;
		subAABB[0]->maxB.x = minB.x + step.x;
		subAABB[0]->maxB.y = minB.y + step.y;
		subAABB[0]->maxB.z = minB.z + step.z;
		//drb
		subAABB[1]->minB.x = minB.x + step.x;
		subAABB[1]->minB.y = minB.y;
		subAABB[1]->minB.z = minB.z;
		subAABB[1]->maxB.x = maxB.x;
		subAABB[1]->maxB.y = minB.y + step.y;
		subAABB[1]->maxB.z = minB.z + step.z;
		//ulb
		subAABB[2]->minB.x = minB.x;
		subAABB[2]->minB.y = minB.y + step.y;
		subAABB[2]->minB.z = minB.z;
		subAABB[2]->maxB.x = minB.x + step.x;
		subAABB[2]->maxB.y = maxB.y;
		subAABB[2]->maxB.z = minB.z + step.z;
		//urb
		subAABB[3]->minB.x = minB.x + step.x;
		subAABB[3]->minB.y = minB.y + step.y;
		subAABB[3]->minB.z = minB.z;
		subAABB[3]->maxB.x = maxB.x;
		subAABB[3]->maxB.y = maxB.y;
		subAABB[3]->maxB.z = minB.z + step.z;
		//dlf
		subAABB[4]->minB.x = minB.x;
		subAABB[4]->minB.y = minB.y;
		subAABB[4]->minB.z = minB.z + step.z;
		subAABB[4]->maxB.x = minB.x + step.x;
		subAABB[4]->maxB.y = minB.y + step.y;
		subAABB[4]->maxB.z = maxB.z;
		//drf
		subAABB[5]->minB.x = minB.x + step.x;
		subAABB[5]->minB.y = minB.y;
		subAABB[5]->minB.z = minB.z + step.z;
		subAABB[5]->maxB.x = maxB.x;
		subAABB[5]->maxB.y = minB.y + step.y;
		subAABB[5]->maxB.z = maxB.z;
		//ulf
		subAABB[6]->minB.x = minB.x;
		subAABB[6]->minB.y = minB.y + step.y;
		subAABB[6]->minB.z = minB.z + step.z;
		subAABB[6]->maxB.x = minB.x + step.x;
		subAABB[6]->maxB.y = maxB.y;
		subAABB[6]->maxB.z = maxB.z;
		//urf
		subAABB[7]->minB.x = minB.x + step.x;
		subAABB[7]->minB.y = minB.y + step.y;
		subAABB[7]->minB.z = minB.z + step.z;
		subAABB[7]->maxB.x = maxB.x;
		subAABB[7]->maxB.y = maxB.y;
		subAABB[7]->maxB.z = maxB.z;

		for (auto i = transfTriangles.begin(); i != transfTriangles.end(); i++)
		{
			glm::dvec3 p;
			glm::dvec3 v0 = (*i)->v[0]->pos;
			glm::dvec3 v1 = (*i)->v[1]->pos;
			glm::dvec3 v2 = (*i)->v[2]->pos;
			p.x = (v0.x + v1.x + v2.x) / 3.0;
			p.y = (v0.y + v1.y + v2.y) / 3.0;
			p.z = (v0.z + v1.z + v2.z) / 3.0;

			bool salir = false;
			int bb = 0;
			while (!salir && (bb < 8))
			{
				if ((subAABB[bb]) && subAABB[bb]->testCollisionPoint(p))
				{
					subAABB[bb]->transfTriangles.push_back(*i);
					salir = 1;
				}
				bb++;
			}
			if (!salir)
			{
				std::cout << "ERROR AABB 113\n";
			}
		}
		for (int i = 0; i < 8; i++)
		{
			if (subAABB[i] && (subAABB[i]->transfTriangles.size() > 0) && (subAABB[i]->transfTriangles.size() != transfTriangles.size()))
			{
				subAABBs.push_back(subAABB[i]);
			}
			else
			{
				delete subAABB[i];
			}
		}

		for (auto i = subAABBs.begin(); i != subAABBs.end(); i++)
		{
			(*i)->resize();
			(*i)->subdivide();
		}
	}



}

inline void AABB::resize()
{
	//check bounds
	minB = glm::vec3(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
	maxB = glm::vec3(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);
	for (auto i = transfTriangles.begin(); i != transfTriangles.end(); i++)
	{
		for (int vt = 0; vt < 3; vt++)
		{
			glm::vec3 v = (*i)->v[vt]->pos;
			if (v.x < minB.x)	minB.x = v.x;
			if (v.y < minB.y)	minB.y = v.y;
			if (v.z < minB.z)	minB.z = v.z;
			if (v.x > maxB.x)	maxB.x = v.x;
			if (v.y > maxB.y)	maxB.y = v.y;
			if (v.z > maxB.z)	maxB.z = v.z;
		}
	}
}

 void AABB::getCollisions(std::map<double, collision_t>& arrayColls, collisionRay_t ray)
{	bool coll = testCollisionAABB(ray);
	if (coll) {
		if (subAABBs.size() > 0)
		{
			for (auto i = subAABBs.begin(); i != subAABBs.end();
				i++)
			{
				(*i)->getCollisions(arrayColls, ray);
			}
		}
		else
		{
			collision_t collision;
			for (auto i = transfTriangles.begin(); i != transfTriangles.end(); i++)
			{
				if (triangleTestRay((*i),mat, ray, collision))
				{
					arrayColls[collision.distance]=(collision);
				}
			}
		}
	}
}

inline bool AABB::testCollisionAABB(collisionRay_t r)
{
		float tmin = (minB.x - r.origin.x) / r.dir.x;
		float tmax = (maxB.x - r.origin.x) / r.dir.x;

		if (tmin > tmax) std::swap(tmin, tmax);

		float tymin = (minB.y - r.origin.y) / r.dir.y;
		float tymax = (maxB.y - r.origin.y) / r.dir.y;

		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (minB.z - r.origin.z) / r.dir.z;
		float tzmax = (maxB.z - r.origin.z) / r.dir.z;

		if (tzmin > tzmax) std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	
}

 bool AABB::triangleTestRay(triangle_t* triangle, Material* mat, collisionRay_t ray, collision_t& collision)
{

	 constexpr float kEpsilon = 1e-8;

	// compute plane's normal
	glm::vec3 v0 = triangle->v0->pos;
	glm::vec3 v1 = triangle->v1->pos;
	glm::vec3 v2 = triangle->v2->pos;

	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 N = glm::cross(v0v1, v0v2); // N
	glm::vec3 pvec = glm::cross(ray.dir,v0v2);
	float det = glm::dot(v0v1,pvec);

	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < kEpsilon) return false;

	float invDet = 1 / det;

	glm::vec3 tvec = ray.origin - v0;
	float u = glm::dot(tvec,pvec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec,v0v1);
	float v = glm::dot(ray.dir,qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	float t = glm::dot(v0v2,qvec) * invDet;
	
	// check if the triangle is in behind the ray
	if (t < 0) return false; // the triangle is behind 

	glm::dvec3 P = glm::dvec3(ray.origin) + (double)t * glm::dvec3(ray.dir);

	collision.point.color = triangle->v0->color;
	collision.point.normal = (1.0f - u - v) * triangle->v0->normal+ u * triangle->v1->normal + v * triangle->v2->normal;
	collision.point.normal = glm::normalize(collision.point.normal);
	collision.point.pos = glm::vec4(P,1.0f);
	collision.point.texCoords= (1.0f - u - v) * triangle->v0->texCoords + u * triangle->v1->texCoords + v * triangle->v2->texCoords;
	collision.distance = glm::length(P-glm::dvec3(ray.origin));
	collision.mat = mat;
	return true; // this ray hits the triangle
}

inline bool AABB::testCollisionPoint(glm::vec3 p)
{
	return (p.x >= minB[0] && p.x <= maxB[0]) &&
		(p.y >= minB[1] && p.y <= maxB[1]) &&
		(p.z >= minB[2] && p.z <= maxB[2]);
}

inline AABB::~AABB()
{
	for (auto i = subAABBs.begin(); i != subAABBs.end(); i++)
	{
		delete (*i);
	}
	subAABBs.resize(0);
	if (isRoot)
	{
		delete transfVertices;

		for (auto i = triangles.begin(); i != triangles.end(); i++)
		{
			delete (*i);
		}
		for (auto i = transfTriangles.begin(); i != transfTriangles.end(); i++)
		{
			delete (*i);
		}

	}
}
