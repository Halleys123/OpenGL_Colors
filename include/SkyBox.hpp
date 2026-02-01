#pragma once

#include "glad/glad.h"

class SkyBox {
private:
	int width, height, components;
	unsigned int skyBoxID;
	unsigned int skyboxVAO, skyboxVBO;
	
	void setupCubeMesh();
	void convertEquirectangularToCubemap(float* equirectData);
	
public:
	SkyBox(const char* srcFile);
	~SkyBox();
	void Draw();
	void Bind(unsigned int textureUnit = 0);
	unsigned int GetTextureID() const { return skyBoxID; }
};