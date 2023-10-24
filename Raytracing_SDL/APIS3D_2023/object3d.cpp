#define _CRT_SECURE_NO_WARNINGS
#include "object3d.h"
#include <string>
#include <sstream>
#include "pugixml.hpp"
#include <iostream>
#include "state.h"
int Object3D::idCounter=0;
Object3D::Object3D()
{
	this->objId = idCounter;
	idCounter++;
	this->entityType = OBJECT3D;
    //printf("Clase base\n");
    this->pos=this->rot=glm::vec3(0,0,0);
    this->size=glm::vec3(1,1,1);
    this->mesh = new MSH_t;
    mesh->meshList=nullptr;
    mesh->materialList=nullptr;

    this->mesh->meshList = new std::vector<Mesh3D*>();
    this->mesh->materialList = new std::vector<Material*>();

}


Object3D::Object3D(const char* fileName)
{
	this->objId = idCounter;
	idCounter++;

    this->pos=this->rot=glm::vec3(0,0,0);
    this->size=glm::vec3(1,1,1);
    printf("Clase base fichero\n");
    loadMSH(fileName);
}

std::vector<std::string>* splitString(const std::string& str, char delim) {
    std::vector<std::string>* elems = new std::vector<std::string>();
    std::stringstream sstream(str);
    std::string item;
    if (str != "") {
        while (std::getline(sstream, item, delim))
        {
            elems->push_back(item);
        }
    }
    return elems;
}

template <typename T>
T numberFromString(const std::string& str)
{
    T number;
    std::istringstream stream(str);
    stream >> number;
    return number;
}

void Object3D::loadOBJ(const char* fileName)
{
	if (State::mshList.find(std::string(fileName)) == State::mshList.end())
	{
		this->mesh = new MSH_t;
		mesh->meshList = nullptr;
		mesh->materialList = nullptr;

		mesh->meshList = new std::vector<Mesh3D*>();
		mesh->materialList = new std::vector<Material*>();
		
		Mesh3D* mesh = new Mesh3D();
		Material* mat = new Material(NULL, State::defaultShader);

		mat->setReceiveShadows(true);
		mat->setLighting(true);
		mat->setRefraction(false);
		mat->setRefractionCoef(0.75f);
		mat->setReflection(false);
		mat->setShininess(95);

		mat->setColor(glm::vec4(165.0f/255.0f, 42.0f / 255.0f, 42.0f / 255.0f,1));
		mat->setCulling(true);
		mat->setDepthWrite(true);
		FILE* f = fopen(fileName, "rb");
		
		std::string line = "";
		char c = 0;
		std::vector<struct vertex_t>* vertList = mesh->getVertList();
		std::vector<int>* triangleList = mesh->getTriangleIdxList();

		do {
			//readLine
			c = 0;
			line = "";
			while (((c = getc(f)) != '\n')&& !feof(f)) line += c;
			//is a comment
			int vertexCount = 0;
			int normalCount = 0;
			int triangleCount = 0;
			if (line[0] != '#')
			{
				//is a vertex
				if (line[0] == 'v' && line[1] == ' ')
				{
					vertex_t vert;
					vert.color = mat->getColor();
					vert.pos.w = 1.0;
					char type[5];
					int err=sscanf(line.c_str(), "%s %f %f %f", type, &vert.pos.x, &vert.pos.y, &vert.pos.z);
					if (err != 4)
					{
						std::cout << "ERROR loadOBJ 102\n";
					}
					mesh->addVertex(vert);
					vertexCount++;
				}
				//is a normal
				/*if (line[0] == 'v' && line[1] == 'n')
				{
					vertex_t vert= (*vertList)[normalCount];
					char type[5];
					int err=sscanf(line.c_str(), "%s %f %f %f", type, &vert.normal.x, &vert.normal.y, &vert.normal.z);
					if (err != 4)
					{
						std::cout << "ERROR loadOBJ 116\n";
					}
					(*vertList)[normalCount] = vert;
					normalCount++;
				}*/
				//is a triangle
				if (line[0] == 'f' && line[1] == ' ')
				{
					int v1=0, v2=0, v3=0;
					char type[5];
					int err = sscanf(line.c_str(), "%s %d %d %d", type, &v1, &v2, &v3);
					if (err != 4)
					{
						std::cout << "ERROR loadOBJ 116\n";
					}
					mesh->addTriangleIdx(v1-1, v2-1, v3-1);
				}

			}
			

		} while (!feof(f));
		
		fclose(f);

		for(auto it= vertList->begin();it!= vertList->end();it++)
			{  //For each vertex
				//vertex.n : = (0, 0, 0)
				it->normal = glm::vec3(0, 0, 0);
			}
		for (auto it = triangleList->begin(); it != triangleList->end(); ) 

			{	//For each triangle ABC
			//compute the cross product and add it to each vertex
			//p : = cross(B - A, C - A)
			//A.n += p
			//B.n += p
			//C.n += p
				int v1 = *it; it++;
				int v2 = *it; it++;
				int v3 = *it; it++;

				glm::vec3 normal = -glm::cross(glm::vec3((*vertList)[v2].pos - (*vertList)[v1].pos),
											  glm::vec3((*vertList)[v3].pos - (*vertList)[v1].pos));
				(*vertList)[v1].normal += normal;
				(*vertList)[v2].normal += normal;
				(*vertList)[v3].normal += normal;
			}
			//For each vertex
			//vertex.n : = normalize(vertex.n)
		for (auto it = vertList->begin(); it != vertList->end(); it++)
			{  //For each vertex
				//vertex.n : = (0, 0, 0)
				it->normal = glm::normalize(it->normal);
			}
		this->addMesh(mesh, mat);

		State::mshList[std::string(fileName)] = this->mesh;
	}
	else
	{
		this->mesh = State::mshList[std::string(fileName)];
	}
}

void Object3D::loadMSH(const char* fileName)
{

	if (State::mshList.find(std::string(fileName)) == State::mshList.end())
	{
        this->mesh = new MSH_t;
        mesh->meshList=nullptr;
        mesh->materialList=nullptr;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(fileName);

		mesh->meshList=new std::vector<Mesh3D*>();
		mesh->materialList = new std::vector<Material*>();


		if (this->mesh->meshList->size() > 0)
		{
			for (unsigned int i = 0; i < mesh->meshList->size(); i++)
			{
				delete mesh->meshList->back();
				mesh->meshList->pop_back();
				delete mesh->materialList->back();
				mesh->materialList->pop_back();
			}
		}

		if (result)
		{

			pugi::xml_node meshNode = doc.child("mesh");
			pugi::xml_node buffersNode = meshNode.child("buffers");
			//por cada hijo buffer, crear una malla3d con su material
			for (pugi::xml_node bufferNode = buffersNode.child("buffer");
				bufferNode;
				bufferNode = bufferNode.next_sibling()
				)
			{
				Mesh3D* mesh = new Mesh3D();
				Material* mat = new Material(NULL, State::defaultShader);
				if (bufferNode.child("material").child("texture"))
				{
					GLTexture* tex = nullptr;
					if (bufferNode.child("material").child("texture").attribute("cubeMap"))
						if (bufferNode.child("material").child("texture").attribute("cubeMap").as_bool())
						{
							tex = new GLTexture();
							std::vector<std::string>* textures = splitString(bufferNode.child("material").child("texture").text().as_string(), ',');
							tex->load(textures->at(0).c_str(), textures->at(1).c_str(), textures->at(2).c_str(),
								textures->at(3).c_str(), textures->at(4).c_str(), textures->at(5).c_str());
							delete textures;
						}
						else
						{
							tex = new GLTexture(bufferNode.child("material").child("texture").text().as_string());
						}
					else {
						tex= new GLTexture(bufferNode.child("material").child("texture").text().as_string());
					}

					 
					mat->setTexture(tex);
				}

				if (bufferNode.child("material").child("normal_texture"))
				{
					GLTexture* tex = new GLTexture(bufferNode.child("material").child("normal_texture").text().as_string());
					mat->setNormalTexture(tex);
				}

				glm::vec4 color = glm::vec4(1, 1, 1, 1);
				if (bufferNode.child("material").child("color"))
				{
					
					std::vector<std::string>* vcolor = splitString(bufferNode.child("material").child("color").text().as_string(), ',');
					std::vector<std::string>::iterator itColor = vcolor->begin();
					for (int i = 0; i < 4 && itColor != vcolor->end(); i++, itColor++)
					{
						color[i] = numberFromString<float>(*itColor);
					}
					delete vcolor;
					mat->setColor(color);
				}
				if (bufferNode.child("material").child("shininess"))
				{
					mat->setShininess(bufferNode.child("material").child("shininess").text().as_int());
				}
				else
					mat->setShininess(32);

				if (bufferNode.child("material").child("light"))
				{
					mat->setLighting(bufferNode.child("material").child("light").text().as_bool());
				}
				else
					mat->setLighting(true);

				if (bufferNode.child("material").child("receiveShadow"))
				{
					mat->setReceiveShadows(bufferNode.child("material").child("receiveShadow").text().as_bool());
				}
				else
					mat->setReceiveShadows(true);

				if (bufferNode.child("material").child("reflection").text().as_bool())
				{
					mat->setReflection(true);
					if (bufferNode.child("material").child("reflection").text().as_bool())
					{
						GLTexture* tex = new GLTexture();
						if (bufferNode.child("material").child("texture").attribute("cubeMap").as_bool())
						{
							std::vector<std::string>* textures = splitString(bufferNode.child("material").child("texture").text().as_string(), ',');
							tex->load(textures->at(0).c_str(), textures->at(1).c_str(), textures->at(2).c_str(),
								textures->at(3).c_str(), textures->at(4).c_str(), textures->at(5).c_str());
							mat->setReflectionTexture(tex);
							delete textures;
						}
					}
				}
				else
					mat->setReflectionTexture(NULL);

				if (bufferNode.child("material").child("refraction").text().as_bool())
				{
					mat->setRefraction(true);
					if (bufferNode.child("material").child("texture").attribute("cubeMap").as_bool())
					{
						GLTexture* tex = new GLTexture();
						std::vector<std::string>* textures = splitString(bufferNode.child("material").child("texture").text().as_string(), ',');
						tex->load(textures->at(0).c_str(), textures->at(1).c_str(), textures->at(2).c_str(),
							textures->at(3).c_str(), textures->at(4).c_str(), textures->at(5).c_str());
						mat->setRefractionTexture(tex);
						delete textures;

					}
					if (bufferNode.child("material").child("refract_coef"))
					{
						mat->setRefractionCoef(bufferNode.child("material").child("refract_coef").text().as_float());
					}
					else
					{
						mat->setRefractionCoef(0.5f);
					}
				}
				else
					mat->setRefractionTexture(NULL);

				if (bufferNode.child("material").child("blendMode"))
				{
					std::string bmode = bufferNode.child("material").child("blendMode").text().as_string();
					if (bmode == "alpha")
						mat->setBlendMode(Material::ALPHA);
					if (bmode == "add")
						mat->setBlendMode(Material::ADD);
					if (bmode == "mul")
						mat->setBlendMode(Material::MUL);
				}
				else
					mat->setBlendMode(Material::ALPHA);


				if (bufferNode.child("material").child("culling"))
				{
					mat->setCulling(bufferNode.child("material").child("culling").text().as_bool());
				}
				else
					mat->setCulling(true);

				if (bufferNode.child("material").child("depthWrite"))
				{
					mat->setDepthWrite(bufferNode.child("material").child("depthWrite").text().as_bool());
				}
				else
					mat->setDepthWrite(true);


				std::vector<std::string>* coords = splitString(bufferNode.child("coords").text().as_string(), ',');
				std::vector<std::string>* texCoords = splitString(bufferNode.child("texCoords").text().as_string(), ',');
				std::vector<std::string>* indices = splitString(bufferNode.child("indices").text().as_string(), ',');
				std::vector<std::string>* normals = splitString(bufferNode.child("normals").text().as_string(), ',');
				std::vector<std::string>* tangents = splitString(bufferNode.child("tangents").text().as_string(), ',');

				int coordsCompCount = 3;// bufferNode.child("coords").attribute("vertexCompCount").as_int();;
				int texCoordCompCount = 2;
				int normalsCompCount = 3;
				int tangentCompCount = 3;

				if (bufferNode.child("coords").attribute("vertexCompCount"))
					texCoordCompCount = bufferNode.child("coords").attribute("vertexCompCount").as_int();
				else
					texCoordCompCount = 3;
				

				if (bufferNode.child("texCoords").attribute("texCoordCompCount"))
					texCoordCompCount = bufferNode.child("texCoords").attribute("texCoordCompCount").as_int();
				else
					texCoordCompCount = 2;

				if (bufferNode.child("normals").attribute("normalCompCount"))
					normalsCompCount = bufferNode.child("normals").attribute("normalCompCount").as_int();
				else
					normalsCompCount = 3;

				if (bufferNode.child("tangents").attribute("tangentCompCount"))
					tangentCompCount = bufferNode.child("tangents").attribute("tangentCompCount").as_int();
				else
					tangentCompCount = 3;


				std::vector<std::string>::iterator itCoords = coords->begin();
				std::vector<std::string>::iterator itTextCoords = texCoords->begin();
				std::vector<std::string>::iterator itIndexes = indices->begin();
				std::vector<std::string>::iterator itNormals = normals->begin();
				std::vector<std::string>::iterator itTangents = tangents->begin();

				while (itCoords != coords->end())
				{
					vertex_t vert;
					vert.color = color;
					vert.pos.w = 1.0;
					for (int i = 0; i < coordsCompCount && itCoords != coords->end(); i++, itCoords++)
					{
						vert.pos[i] = numberFromString<float>(*itCoords);
					}
					for (int i = 0; i < texCoordCompCount && itTextCoords != texCoords->end(); i++, itTextCoords++)
					{
						vert.texCoords[i] = numberFromString<float>(*itTextCoords);
					}
					for (int i = 0; i < normalsCompCount && itNormals != normals->end(); i++, itNormals++)
					{
						vert.normal[i] = numberFromString<float>(*itNormals);
					}
					for (int i = 0; i < tangentCompCount && itTangents != tangents->end(); i++, itTangents++)
					{
						vert.tangent[i] = numberFromString<float>(*itTangents);
					}
					mesh->addVertex(vert);
				}

				while (itIndexes != indices->end())
				{
					int v1 = numberFromString<int>(*itIndexes);
					itIndexes++;
					int v2 = numberFromString<int>(*itIndexes);
					itIndexes++;
					int v3 = numberFromString<int>(*itIndexes);
					itIndexes++;
					mesh->addTriangleIdx(v1, v2, v3);
				}

				this->addMesh(mesh, mat);

				delete(coords);
				delete(texCoords);
				delete(indices);
				delete(normals);

			}


		}
		else
		{
			std::cout << result.description() << "\n";

		}
		//añadir a la lista compartida
		State::mshList[std::string(fileName)] = this->mesh;
	}
	else
	{
	   this->mesh =State::mshList[std::string(fileName)] ;
	}


}

//Métodos para inicializar/acceder a la malla
void Object3D::addMesh(Mesh3D* mesh, Material *material)
{
    this->mesh->meshList->push_back(mesh);
    this->mesh->materialList->push_back(material);
	aabbs.push_back(new AABB(mesh,material));
}

Mesh3D* 	Object3D::getMesh(int pos)
{
    return this->mesh->meshList->at(pos);
}

Material*	Object3D::getMaterial (int pos)
{
    return mesh->materialList->at(pos);
}

void Object3D::updateAABB(glm::mat4 mat, int idx)
{
	this->aabbs[idx]->update(mat);
}


int Object3D::getMeshCount()
{
    return (int)mesh->meshList->size();
}

int Object3D::getMaterialCount()
{
   return (int) mesh->materialList->size();
}
