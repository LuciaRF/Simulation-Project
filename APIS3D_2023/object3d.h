#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "mesh3d.h"
#include "shader.h"
#include "entity.h"
#include "material.h"
#include "AABB.h"

typedef struct MSH_t
{
	// malla 3d de este objeto
	std::vector<Mesh3D*>* meshList;

	//Shader para dibujar este objeto
	std::vector<Material*>* materialList;
}MSH_t;

class Object3D : public Entity
{
protected:
    int objId;
	MSH_t* mesh;
    std::vector<AABB*> aabbs;
    static int idCounter;
public:
    Object3D();
    Object3D(const char* fileName);
    //Método virtual, implementado por los objetos que hereden de esta clase
    virtual void 	step(float deltatime)=0;
    //Métodos para inicializar/acceder a la malla
    void addMesh(Mesh3D* mesh, Material* material) ;
    Mesh3D* 	getMesh(int pos);
    AABB* getAABB(int pos) { return aabbs[pos]; }
    //Métodos para inicializar/acceder al shader del objeto
    Material *	getMaterial (int pos);
    void updateAABB(glm::mat4 mat, int idx);
    void loadMSH(const char* filename);
    void loadOBJ(const char* filename);

    int getId() { return objId; }

    int getMeshCount();
    int getMaterialCount();
};

#endif // OBJECT3D_H
