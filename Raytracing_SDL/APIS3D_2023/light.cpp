#include "light.h"
#include "state.h"

Light::Light() {
	this->setColor(glm::vec3(1, 1, 1));
	this->setPos(glm::vec3(0, 0, 0));
	this->setLinearAttenuation(1);
	this->setType(Light::DIRECTIONAL);
} 

Light::Light(glm::vec3 position, Type  type)
{
	this->entityType = LIGHT;
	this->setColor(glm::vec3(1, 1, 1));
	this->setPos(position);
	this->setLinearAttenuation(1);
	this->setType(type);
}
Light::Type			Light::getType() const {
	return type;
}


void Light::setType(Light::Type type)
{
	this->type = type;

}
const glm::vec3& Light::getColor() const {
	return color;
}
void Light::setColor(const glm::vec3& color) {
	this->color = color;
}

float	Light::getLinearAttenuation() const {
	return linearAttenuation;
}
void			Light::setLinearAttenuation(float att) {
	this->linearAttenuation = att;
}
void	Light::prepare(int index, GLSLProgram* shader) const {

	//shader->setInt(shader->getLocation("enableLight"), 1);
	shader->setVec3("lightPos", this->pos);
	shader->setVec3("lightColor", this->color);
	shader->setVec3("ambientLight", State::ambient);
}

void Light::step(float timestep) {}

Light::~Light() {
} 
