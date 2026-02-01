#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "stb_image.h"

#include "GlHelp.h"

#include "Texture.hpp"

Texture::Texture(const char* texturePath, GLenum TextureType, GLenum InternalFormat, GLenum ExternalFormat, int requiredComponents) : TextureID(0), TextureType(TextureType){
	if (!texturePath) {
		throw std::invalid_argument("Texture path not provide, Please provide a valid texture path");
		OutputDebugString("Texture path not provide, Please provide a valid texture path");
		return;
	}
	stbi_set_flip_vertically_on_load(1);
	unsigned char* image = stbi_load(texturePath, &width, &height, &components, requiredComponents);
	
	if (!image) {
		OutputDebugString("No image found at provided path");
		throw std::exception("No image found at provided path");
		return;
	}

	GlCall(glGenTextures(1, &this->TextureID));
	GlCall(glBindTexture(GL_TEXTURE_2D, TextureID));
	
	glGenSamplers(1, &this->SamplerID);

	glSamplerParameteri(this->SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(this->SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(this->SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(this->SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned int pbo;
	GlCall(glGenBuffers(1, &pbo));
	GlCall(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
	GlCall(glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * components * sizeof(unsigned char), image, GL_STATIC_DRAW));

	GlCall(glTexStorage2D(this->TextureType, 1, InternalFormat, width, height));
	GlCall(glTexSubImage2D(this->TextureType, 0, 0, 0, width, height, ExternalFormat, GL_UNSIGNED_BYTE, 0));
	//GlCall(glTexImage2D(this->TextureType, 0, InternalFormat, width, height, 0, ExternalFormat, GL_UNSIGNED_BYTE, image));
    GlCall(glBindTexture(this->TextureType, 0));

	GlCall(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
	GlCall(glDeleteBuffers(1, &pbo));
    GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    stbi_image_free(image);
}

void Texture::activate(int textureUnit) {
	if (glIsTexture(this->TextureID) == GL_FALSE) {
		printf("[OpenGL Error] :: This texture object is deleted or not aviable.\n");
		return;
	}
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(this->TextureType, this->TextureID);
	glBindSampler(textureUnit, this->SamplerID);
}

Texture::~Texture() {
	glDeleteTextures(1, &this->TextureID);
}
