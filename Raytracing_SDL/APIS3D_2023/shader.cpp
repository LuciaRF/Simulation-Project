#include "shader.h"

GLSLShader::GLSLShader(std::string fileName) {
    this->fileName = fileName;

    if (fileName.ends_with(".vertex"))
        type = GL_VERTEX_SHADER;
    if (fileName.ends_with(".fragment"))
        type = GL_FRAGMENT_SHADER;
    if (fileName.ends_with(".compute"))
        type = GL_COMPUTE_SHADER;

    idShader = glCreateShader(type);

    readFile();
    compileShader();
}

void GLSLShader::readFile() {
    std::ifstream f(fileName);
    if (f.is_open()) {
        code = std::string(std::istreambuf_iterator<char>(f), {});
    }
    else {
        std::cout << "ERROR: FICHERO NO ENCONTRADO "<<__FILE__<<":"<<__LINE__<<" " << fileName << "\n";
    }
}

void GLSLShader::compileShader() {
    const char* src = code.c_str();
    glShaderSource(idShader, 1, &src, nullptr);
    glCompileShader(idShader);

    //detección errores compilado
    checkShaderErrors();
}

void GLSLShader::checkShaderErrors() {

    GLint fragment_compiled;
    glGetShaderiv(idShader, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(idShader, 1024, &log_length, message);
        std::cout << "ERROR " << fileName << "\n" << message << "\n\n";
    }

}

GLSLShader::~GLSLShader()
{
    glDeleteShader(idShader);

}

GLSLProgram::GLSLProgram() {

    idProgram = glCreateProgram();
   // glProgramParameteri(idProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
}

void GLSLProgram::addShader(std::string fileName) {

    GLSLShader* sh = new GLSLShader(fileName);
   // sh->compileShader();
    shaders.push_back(sh);

}

void GLSLProgram::linkProgram() {
    //glUseProgram(idProgram);
    for(auto i: shaders)
        glAttachShader(idProgram, i->idShader);
    glLinkProgram(idProgram);
    //revisar errores
    checkLinkerErrors();
    readVarList();
    //glGenProgramPipelines(1, &pipeLineID);
    //unsigned int FLAGS=0;
    //for(auto i: shaders)
    //{
    //    if(i->type==GL_COMPUTE_SHADER) FLAGS|=GL_COMPUTE_SHADER_BIT;
    //    if(i->type==GL_VERTEX_SHADER) FLAGS|=GL_VERTEX_SHADER_BIT;
    //    if(i->type==GL_FRAGMENT_SHADER) FLAGS|=GL_FRAGMENT_SHADER_BIT;
    //}
    //glUseProgramStages(pipeLineID, FLAGS, idProgram);
}

void GLSLProgram::use()
{
    glUseProgram(idProgram);

}

void GLSLProgram::checkLinkerErrors()
{
    GLint program_linked;
    glGetProgramiv(idProgram, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(idProgram, 1024, &log_length, message);
        std::cout << "ERROR \n" << message << "\n\n";
    }

}

void GLSLProgram::readVarList(){

    int numAttributes = 0;
    int numUniforms = 0;
    glGetProgramiv(idProgram, GL_ACTIVE_ATTRIBUTES, &numAttributes);
    for (int i = 0; i < numAttributes; i++)
    {
        char varName[100];
        int bufSize = 100, length = 0, size = 0;
        GLenum type = -1;
        glGetActiveAttrib(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName);
        varList[std::string(varName)] = glGetAttribLocation(idProgram,varName);
    }

    glGetProgramiv(idProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (int i = 0; i < numUniforms; i++)
    {
        char varName[100];
        int bufSize = 100, length = 0, size = 0;
        GLenum type = -1;
        glGetActiveUniform(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName);
        varList[std::string(varName)] = glGetUniformLocation(idProgram, varName);
    }
}

unsigned int GLSLProgram::getVarLocation(std::string varName) {
    if (varList.find(varName) != varList.end())
        return varList[varName];
    else {
        std::cout << "ERROR: variable "<< varName<<" no encontrada en shader\n";
        return -1;
    }

}

void GLSLProgram::setAttribute(unsigned int location, void *description, int compCount) {

    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, compCount, GL_FLOAT, GL_FALSE, sizeof(vertex_t), description);

}

void GLSLProgram::setUniformMatrix4x4(unsigned int location, float *matrix) {
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix);

}

void GLSLProgram::setVertexPositions(void *description, int compCount) {

    setAttribute(getVarLocation("vpos"), description,compCount);

}

void GLSLProgram::setVertexColors(void *description, int compCount) {
    setAttribute(getVarLocation("vcolor"), description,compCount);
}

void GLSLProgram::setVertexNormals(void *description, int compCount) {
    setAttribute(getVarLocation("vnormal"), description,compCount);
}

void GLSLProgram::setVertexTangents(void *description, int compCount) {
    setAttribute(getVarLocation("vtangent"), description,compCount);
}

void GLSLProgram::setVertexUVs(void *description, int compCount) {
    setAttribute(getVarLocation("vtex"), description,compCount);
}

void GLSLProgram::setMatrixMVP(float *matrix) {
    setUniformMatrix4x4(getVarLocation("MVP"), matrix);
}

void GLSLProgram::setMatrixM(float *matrix) {
    setUniformMatrix4x4(getVarLocation("ModelMatrix"), matrix);
}

void GLSLProgram::setInt(std::string name, int val)
{
    glUniform1i(getVarLocation(name),val);

}

void GLSLProgram::setFloat(std::string name, float val)
{
    glUniform1f(getVarLocation(name),val);

}

void GLSLProgram::setVec3(std::string name, const glm::vec3 &vec)
{
    glUniform3fv(getVarLocation(name), 1, (float*)&(vec[0]));

}

void GLSLProgram::setVec4(std::string name, const glm::vec4 &vec)
{
    glUniform4fv(getVarLocation(name), 1, (float*)&(vec[0]));

}

void GLSLProgram::setMatrix(std::string name, const glm::mat4 &matrix)
{
    setUniformMatrix4x4(getVarLocation(name), (float*)&matrix[0][0]);
}
/*
void GLSLProgram::setLight(light_t &light)
{
    glUniform4fv(getVarLocation("light.position"), 1, (float*)&(light.position.x));
    glUniform4fv(getVarLocation("light.color"), 1, (float*)&(light.color.x));
    glUniform4fv(getVarLocation("light.direction"), 1, (float*)&(light.direction.x));
    glUniform1i (getVarLocation("light.type"), light.type);
    glUniform1f(getVarLocation("light.ka"), light.ka);
    glUniform1f(getVarLocation("light.kd"), light.kd);
    glUniform1f(getVarLocation("light.ks"), light.ks);

}

void GLSLProgram::setMaterial(material_t &mat)
{
    glUniform1i(getVarLocation("materials.textureUnitId"), mat.textureUnitID);
    glUniform1i(getVarLocation("materials.enabled"), mat.enabled);

}*/
