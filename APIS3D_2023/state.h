#ifndef STATE_H
#define STATE_H
#include "shader.h"
#include "light.h"
#include "camera.h"
#include <glm/glm.hpp>
#include "object3d.h"
#include <string>
class State
{


public:

    static GLSLProgram*	defaultShader;
	static GLSLProgram* activeShader;
    static glm::mat4 				projectionMatrix;
    static glm::mat4 				viewMatrix;
    static glm::mat4 				modelMatrix;
	static std::vector<Light* > lights;
	static Camera*		activeCamera;
	static glm::vec3	ambient;
	static std::map<std::string,MSH_t* > mshList;
	static float currentTime;
	static float lastTime;    static float deltaTime;

private:
        State();
};

#endif // STATE_H
