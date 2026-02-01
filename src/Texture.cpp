#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "stb_image.h"

#include "GlHelp.h"

#include "Texture.hpp"

Texture::Texture(const char* texturePath, int requiredComponents) : TextureID(0) {
	if (!texturePath) {
		throw std::invalid_argument("Texture path not provide, Please provide a valid texture path");
		return;
	}
	stbi_set_flip_vertically_on_load(1);
	unsigned char* image = stbi_load(texturePath, &width, &height, &components, requiredComponents);
	
	if (!image) {
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

	GLenum internal_format = (components == 4) ? GL_RGBA8 : (components == 3) ? GL_RGB8 : GL_RED;
	GLenum external_format = (internal_format == GL_RGBA8) ? GL_RGB : GL_RGB;
	unsigned int pbo;
	GlCall(glGenBuffers(1, &pbo));
	GlCall(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
	GlCall(glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * components * sizeof(unsigned char), image, GL_STATIC_DRAW));

	GlCall(glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height));
	GlCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, external_format, GL_UNSIGNED_BYTE, 0));
	//GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image));
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));

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
	glBindTexture(GL_TEXTURE_2D, this->TextureID);
	glBindSampler(textureUnit, this->SamplerID);
}

Texture::~Texture() {
	glDeleteTextures(1, &this->TextureID);
}
