#ifndef MATERIAL_H
#define MATERIAL_H
#include "gltexture.h"
#include "shader.h"

#define SAMPLER_COLOR_TEXT 0
#define SAMPLER_CUBE_TEXT 1
#define SAMPLER_NORMAL_TEXT 2
#define SAMPLER_REFLECT_TEXT 3
#define SAMPLER_REFRACT_TEXT 4

class Material
{
public:
	enum BlendMode
	{
		ALPHA, ADD, MUL
	};
private:
	GLTexture* tex=NULL;
	GLTexture* reflectionTex = NULL;
	GLTexture* refractionTex = NULL;
	GLTexture* normalTex = NULL;
	float refractionCoef;

	GLSLProgram* shader = NULL;
	glm::vec4 color;
	int shininess;
	Material::BlendMode blendMode=ALPHA;
	bool receiveShadows = false;
	bool lightEnable = false;
	bool cullingEnable = false;
	bool depthWriteEnable = false;
	bool reflectionEnable = false;
	bool refractionEnable = false;
public:

	Material();
	Material(GLTexture* tex = nullptr, GLSLProgram* shader = nullptr)
	{
		this->shader = shader;
		reflectionTex = refractionTex = normalTex = nullptr;
		refractionCoef = 0;
		this->tex = tex;
		shininess = 0;
		color = glm::vec4(1, 0, 0, 1);
	}
	GLSLProgram* getShader() const { return shader; }

	void setShader(GLSLProgram* shader) { this->shader = shader; }


	void prepare(glm::mat4x4 ModelMatrix, glm::mat4x4 ViewMAtrix, glm::mat4x4 ProjMatrix);

	const glm::vec4& getColor() const { return color; }
	void  setColor(const glm::vec4& color) { this->color = color; }

	int	getShininess() const { return shininess; }
	void	setShininess(int shininess) { this->shininess = shininess; }

	BlendMode	getBlendMode() const { return blendMode; }
	void		setBlendMode(BlendMode blendMode){this->blendMode = blendMode;}
	bool		getLighting() const { return lightEnable; }
	void		setLighting(bool enable) { this->lightEnable = enable; }
	bool		getReceiveShadows() const { return receiveShadows; }

	void		setReceiveShadows(bool enable) { this->receiveShadows = enable; }
	
	bool		getCulling() const { return cullingEnable; }
	void		setCulling(bool enable) { this->cullingEnable = enable; }
	bool		getDepthWrite() const { return depthWriteEnable; }
	void		setDepthWrite(bool enable) { this->depthWriteEnable = enable; }

	GLTexture* getReflectionTexture() const { return reflectionTex; }
	void setReflectionTexture(GLTexture* tex) { this->reflectionTex = tex; }
	void setReflection(bool enable) {
		reflectionEnable = enable;
	}

	bool getReflection()
	{
		return reflectionEnable;
	}


	GLTexture* getRefractionTexture() const { return refractionTex; }
	void setRefractionTexture(GLTexture* tex) { this->refractionTex = tex; }
	void setRefraction(bool enable) {
		refractionEnable = enable;
	}

	bool getRefraction()
	{
		return refractionEnable;
	}



	GLTexture* getNormalTexture() const { return normalTex; }
	void setNormalTexture(GLTexture* tex) { this->normalTex = tex; }

	float getRefractionCoef() const { return refractionCoef; }
	void setRefractionCoef(float coef) { this->refractionCoef = coef; }

	GLTexture* getTexture() const { return tex; }
	void setTexture(GLTexture* tex) { this->tex = tex; }

	

};

#endif // MATERIAL_H
