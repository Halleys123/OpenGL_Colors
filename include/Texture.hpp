#pragma once

#include "glad/glad.h"

class Texture {
private:
	int width, height, components;
	GLenum TextureType;
	unsigned int TextureID;
	unsigned int SamplerID;
public:
	Texture(const char* texturePath, GLenum TextureType, GLenum InternalFormat = GL_RGB8, GLenum ExternalFormat = GL_RGB, int requiredComponents = 0);
	~Texture();
	void activate(int textureUnit = 0);
};