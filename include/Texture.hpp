#pragma once

class Texture {
private:
	int width, height, components;
	unsigned int TextureID;
public:
	Texture(const char* texturePath, int requiredComponents = 0);
};