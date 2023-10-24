#pragma once
#include "entity.h"
#include "shader.h"

class Light : public Entity
{
	
	
	glm::vec3 color;
	float linearAttenuation;

public:
	enum Type {
		POINT, DIRECTIONAL
	};

	Type type;

	Light();
	Light(glm::vec3 position, Type  type);
	Type			getType() const;
	void			setType(Type type);
	const glm::vec3& getColor() const;
	void			setColor(const glm::vec3& color);
	float			getLinearAttenuation() const;
	void			setLinearAttenuation(float att);
	void			prepare(int index, GLSLProgram* shader) const;

	void step(float timestep);

	~Light();


};

