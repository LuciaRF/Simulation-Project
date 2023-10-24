#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#include <glm/glm.hpp>


class GLTexture
{
    uint32_t textID;
    glm::ivec2 size;
	bool cubemap = false;
	unsigned char* texBytes = 0x00;
	unsigned char* texLeft = 0x00;
	unsigned char* texRight = 0x00;
	unsigned char* texFront = 0x00;
	unsigned char* texBack = 0x00;
	unsigned char* texUp = 0x00;
	unsigned char* texDown = 0x00;
	


public:
	GLTexture();
    GLTexture(const char* filename);
    void 		load(const char* filename);
	void load(
		const char* left, const char* right,
		const char* front, const char* back,
		const char* top, const char* bottom);

    uint32_t					getId() const{return textID;}
    const glm::ivec2&				getSize() const{return size;}
	void  bind(size_t layer) const;
	bool isCube() { return cubemap; }
	unsigned char* getText2DBytes() {
		return texBytes;
	}
	glm::vec4 getColorAtXY(int x, int y)
	{
		glm::vec4 color;
		color.r = ((float)texBytes[(y * size.x + x) * 4])/255.0f;
		color.g = ((float)texBytes[(y * size.x + x) * 4 + 1]) / 255.0f;
		color.b = ((float)texBytes[(y * size.x + x) * 4 + 2]) / 255.0f;
		color.a = ((float)texBytes[(y * size.x + x) * 4 + 3]) / 255.0f;
		return color;
	}
};

#endif // GLTEXTURE_H
