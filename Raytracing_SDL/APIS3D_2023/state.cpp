#include "state.h"



 GLSLProgram*	State::defaultShader;
 GLSLProgram* State::activeShader;
 glm::mat4 		State::projectionMatrix;
 glm::mat4 		State::viewMatrix;
 glm::mat4 		State::modelMatrix;
 std::vector<Light* > State::lights;
 Camera* State::activeCamera;

 glm::vec3	State::ambient;
 std::map<std::string, MSH_t* > State::mshList;
 float State::currentTime;
 float State::lastTime;
 float State::deltaTime;

State::State()
{

}
