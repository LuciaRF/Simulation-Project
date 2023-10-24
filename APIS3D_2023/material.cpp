#include "material.h"
#include "state.h"
Material::Material()
{
	this->shader = NULL;
	this->shininess = 0;
}


void Material::prepare(glm::mat4x4 ModelMatrix, glm::mat4x4 ViewMAtrix, glm::mat4x4 ProjMatrix)
{

	shader->setVertexPositions((void*)offsetof(vertex_t, pos), 4);
	//setup vtex
	shader->setVertexUVs((void*)offsetof(vertex_t, texCoords), 2);
	//setup vnormal
	shader->setVertexNormals((void*)offsetof(vertex_t, normal), 3);
	shader->setVertexTangents((void*)offsetof(vertex_t, tangent), 3);

	//setup texture


	glm::mat4 MVP = ProjMatrix * ViewMAtrix * ModelMatrix;


	shader->setMatrix("MVP", MVP);
	shader->setMatrix("ModelMatrix", ModelMatrix);
	shader->setMatrix("ViewMatrix", ViewMAtrix);
	shader->setMatrix("ProjectionMatrix", ProjMatrix);





	shader->setVec4("vColor", this->color);
	shader->setVec3("eyePos", State::activeCamera->getPos());

	shader->setInt("shininess", this->shininess);
	//texturas "básicas" de color en canal 0


	if (this->tex != NULL)
	{
		shader->setInt("enableColorText", 1);

		if (tex->isCube())
		{
			tex->bind(SAMPLER_CUBE_TEXT);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			shader->setInt("enableCubeText", 1);
		}
		else {
			tex->bind(SAMPLER_COLOR_TEXT);
			shader->setInt("enableCubeText", 0);
		}

	}
	else
		shader->setInt("enableColorText", 0);

	//normales en el canal 1
	if (this->normalTex != NULL)
	{
		shader->setInt("enableNormalText", 1);
		normalTex->bind(SAMPLER_NORMAL_TEXT);


		glm::mat4 normalMat = glm::transpose(glm::inverse(ViewMAtrix * ModelMatrix));
		shader->setMatrix("normalMat", normalMat);
	}
	else
		shader->setInt("enableNormalText", 0);

	//las reflexiones en el canal 3
	if (this->reflectionTex != NULL)
	{
		shader->setInt("enableColorText", 0);
		reflectionTex->bind(SAMPLER_REFLECT_TEXT);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		shader->setInt("enableReflectionText", 1);
		shader->setVec3("eyePos", State::activeCamera->getPos());
	}
	else
		shader->setInt("enableReflectionText", 0);

	if (this->refractionTex != NULL)
	{
		shader->setInt("enableColorText", 0);
		refractionTex->bind(SAMPLER_REFRACT_TEXT);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		shader->setInt("enableRefractionTex", 1);
		shader->setVec3("eyePos", State::activeCamera->getPos());
		shader->setFloat("refractionCoef", this->refractionCoef);

	}
	else
		shader->setInt("enableRefractionTex", 0);


	if (this->getLighting())
		shader->setInt("enableLight", 1);
	else
		shader->setInt("enableLight", 0);

	for (unsigned int i = 0; i < State::lights.size(); i++)
		State::lights[i]->prepare(i, shader);

	//hay que inicializarlo sí o sí...
	shader->setInt("samplerColor", SAMPLER_COLOR_TEXT);
	shader->setInt("samplerCubeMap", SAMPLER_CUBE_TEXT);
	shader->setInt("samplerNormal", SAMPLER_NORMAL_TEXT);
	shader->setInt("samplerReflectMap", SAMPLER_REFLECT_TEXT);
	shader->setInt("samplerRefractMap", SAMPLER_REFRACT_TEXT);
}
