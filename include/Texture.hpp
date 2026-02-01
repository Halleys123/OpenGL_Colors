#pragma once

class Texture {
private:
	int width, height, components;
	unsigned int TextureID;
	unsigned int SamplerID;
public:
	Texture(const char* texturePath, int requiredComponents = 0);
	~Texture();
	void activate(int textureUnit = 0);
};