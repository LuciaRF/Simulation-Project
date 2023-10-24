#pragma once
#include "vertex.h"

//using namespace lights;

class GLSLShader {
public:
    std::string fileName="";
    std::string code="";
    unsigned int idShader=-1;
    unsigned int type = -1;

    GLSLShader(std::string fileName);
    void readFile();

    void compileShader();


    void checkShaderErrors();
    ~GLSLShader();
};


class GLSLProgram {
public:
    std::vector<GLSLShader*> shaders;
    unsigned int idProgram=-1;
    unsigned int pipeLineID=-1;
    std::map<std::string, unsigned int> varList;

    GLSLProgram();

    void addShader(std::string fileName);
    void linkProgram();

        void use();
    void checkLinkerErrors();
    void readVarList();

    unsigned int getVarLocation(std::string varName);

    void setAttribute(unsigned int location, void* description,  int compCount);

    void setUniformMatrix4x4(unsigned int location, float* matrix);

    void setVertexPositions(void* description, int compCount);
    void setVertexColors(void* description, int compCount);

    void setVertexNormals(void* description, int compCount);
    void setVertexTangents(void *description, int compCount);

    void setVertexUVs(void* description, int compCount);


    void setMatrixMVP(float* matrix);


    void setMatrixM(float* matrix);

//    void setLight(light_t& light);

//    void setMaterial(material_t& mat);


    void	setInt(std::string name, int val);
    void	setFloat(std::string name, float val);
    void	setVec3(std::string name, const glm::vec3& vec);
    void	setVec4(std::string name, const glm::vec4& vec);
    void	setMatrix(std::string name, const glm::mat4& matrix);
    ~GLSLProgram() {
        for (auto i : shaders)
            delete i;
        glDeleteProgram(idProgram);

    }
};
