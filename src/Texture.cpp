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

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLenum format = (components == 4) ? GL_RGBA : (components == 3) ? GL_RGB : GL_RED;

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image));
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));

    GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    stbi_image_free(image);
}