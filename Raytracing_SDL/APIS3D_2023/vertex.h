#pragma once
#define GLAD_ONLY_HEADERS
#include "common.h"

typedef struct vertex_t
{
	glm::vec4 pos;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec4 color;

}vertex_t;