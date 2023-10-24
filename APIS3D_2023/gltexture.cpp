#include "gltexture.h"
#include "glrender.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "renderType.h"

GLTexture::GLTexture()
{
	textID = 0;
	this->size.x = this->size.y = 0;
	cubemap = false;
}
GLTexture::GLTexture(const char* filename)
    {
    textID=0;
	cubemap = false;
    this->size.x=this->size.y=0;
    load(filename);
}

typedef union datTransf
{
	unsigned int num;
	unsigned char bytes[4];

}datTransf;

void GLTexture::load(const char* filename)
{

	//texture_t* newTex = new texture_t;
	this->textID = -1;

	texBytes = stbi_load(filename, &size.x, &size.y, nullptr, 4);
/*#if defined(RAYTRACE)
	//our raytracer works on bgra format
	for (int i = 0; i < size.x * size.y; i++)
	{
		datTransf dat;
		dat= ((datTransf*)texBytes)[i];
		//bgra
		((datTransf*)texBytes)[i].bytes[0] =dat.bytes[2];
		((datTransf*)texBytes)[i].bytes[1] = dat.bytes[1];
		((datTransf*)texBytes)[i].bytes[2] =dat.bytes[0];
		((datTransf*)texBytes)[i].bytes[3] = dat.bytes[3];
	}

#endif
*/
#if defined(OPENGLRENDER) || defined(CUDARENDER)	//generar id de textura
	glGenTextures(1, &textID);
	//hacerla activa
	glBindTexture(GL_TEXTURE_2D, textID);
	//parámetros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBytes);
	//generar mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//liberar datos
	stbi_image_free(texBytes);
#endif
}


void GLTexture::load(
	const char* left, const char* right,
	const char* front, const char* back,
	const char* top, const char* bottom)

{
	this->textID = -1;
	cubemap = true;	

	unsigned char* texLeft = stbi_load(left, &size.x, &size.y, nullptr, 4);
	unsigned char* texRight = stbi_load(right, &size.x, &size.y, nullptr, 4);
	unsigned char* texFront = stbi_load(front, &size.x, &size.y, nullptr, 4);
	unsigned char* texBack = stbi_load(back, &size.x, &size.y, nullptr, 4);
	unsigned char* texUp = stbi_load(top, &size.x, &size.y, nullptr, 4);
	unsigned char* texDown = stbi_load(bottom, &size.x, &size.y, nullptr, 4);
#if defined(OPENGLRENDER) || defined(CUDARENDER)	//generar id de textura

	//generar id de textura
	glGenTextures(1, &this->textID);
	//hacerla activa
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textID);
	//parámetros
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texRight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texLeft);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texFront);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBack);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texUp);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDown);

	//liberar datos
	stbi_image_free(texRight);
	stbi_image_free(texLeft);
	stbi_image_free(texFront);
	stbi_image_free(texBack);
	stbi_image_free(texUp);
	stbi_image_free(texDown);
#endif
}

void	GLTexture::bind(size_t layer) const
{	
	glActiveTexture(GL_TEXTURE0+(int)layer);	
	if (cubemap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->textID);
	}else
		glBindTexture(GL_TEXTURE_2D, this->textID);
}
