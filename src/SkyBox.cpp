#include "SkyBox.hpp"
#include "stb_image.h"
#include <cmath>
#include <algorithm>

SkyBox::SkyBox(const char* srcPath) : skyBoxID(0), skyboxVAO(0), skyboxVBO(0), width(0), height(0), components(0) {

	if (!srcPath) {
		__debugbreak();
		return;
	}

	// Load HDR image with floating point data
	stbi_set_flip_vertically_on_load(false);
	float* imageHDR = stbi_loadf(srcPath, &width, &height, &components, 0);

	if (!imageHDR) {
		// Try loading as regular image
		unsigned char* imageLDR = stbi_load(srcPath, &width, &height, &components, 0);
		if (!imageLDR) {
			__debugbreak();
			return;
		}
		
		// Convert LDR to HDR
		imageHDR = new float[width * height * components];
		for (int i = 0; i < width * height * components; i++) {
			imageHDR[i] = imageLDR[i] / 255.0f;
		}
		stbi_image_free(imageLDR);
	}

	// Generate cube map texture
	glGenTextures(1, &this->skyBoxID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyBoxID);

	// Determine format based on components
	GLenum internalFormat = (components == 4) ? GL_RGBA16F : GL_RGB16F;
	GLenum format = (components == 4) ? GL_RGBA : GL_RGB;
	int faceSize = 512; // Resolution for each cube face

	// Allocate storage for all 6 faces
	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat,
			faceSize, faceSize, 0, format, GL_FLOAT, nullptr);
	}

	// Convert equirectangular to cubemap
	convertEquirectangularToCubemap(imageHDR);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Clean up
	if (imageHDR) {
		stbi_image_free(imageHDR);
	}

	// Setup cube mesh for rendering
	setupCubeMesh();
}

void SkyBox::convertEquirectangularToCubemap(float* equirectData) {
	if (!equirectData) return;

	int faceSize = 512;
	float* faceData = new float[faceSize * faceSize * components];

	// Direction vectors for each face
	struct Face {
		int target;
		float right[3], up[3], forward[3];
	};

	Face faces[6] = {
		// Positive X
		{GL_TEXTURE_CUBE_MAP_POSITIVE_X, {0, 0, -1}, {0, -1, 0}, {1, 0, 0}},
		// Negative X
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, {0, 0, 1}, {0, -1, 0}, {-1, 0, 0}},
		// Positive Y
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, {1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
		// Negative Y
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, {1, 0, 0}, {0, 0, -1}, {0, -1, 0}},
		// Positive Z
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, {1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
		// Negative Z
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}}
	};

	const float PI = 3.14159265359f;

	for (int face = 0; face < 6; face++) {
		for (int y = 0; y < faceSize; y++) {
			for (int x = 0; x < faceSize; x++) {
				// Calculate normalized coordinates (-1 to 1)
				float u = (2.0f * x / (faceSize - 1)) - 1.0f;
				float v = (2.0f * y / (faceSize - 1)) - 1.0f;

				// Calculate 3D direction vector
				float dirX = faces[face].forward[0] + u * faces[face].right[0] + v * faces[face].up[0];
				float dirY = faces[face].forward[1] + u * faces[face].right[1] + v * faces[face].up[1];
				float dirZ = faces[face].forward[2] + u * faces[face].right[2] + v * faces[face].up[2];

				// Normalize direction
				float len = sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
				dirX /= len;
				dirY /= len;
				dirZ /= len;

				// Convert to spherical coordinates
				float theta = atan2f(dirZ, dirX); // Longitude
				float phi = asinf(dirY);          // Latitude

				// Convert to equirectangular UV coordinates
				float equiU = (theta / (2.0f * PI)) + 0.5f;
				float equiV = 0.5f - (phi / PI);  // FLIPPED: was (phi / PI) + 0.5f

				// Sample from equirectangular map
				int srcX = (int)(equiU * width) % width;
				int srcY = (int)(equiV * height) % height;
				
				if (srcX < 0) srcX += width;
				if (srcY < 0) srcY += height;

				int srcIdx = (srcY * width + srcX) * components;
				int dstIdx = (y * faceSize + x) * components;

				for (int c = 0; c < components; c++) {
					faceData[dstIdx + c] = equirectData[srcIdx + c];
				}
			}
		}

		// Upload face data
		GLenum format = (components == 4) ? GL_RGBA : GL_RGB;
		glTexSubImage2D(faces[face].target, 0, 0, 0, faceSize, faceSize,
			format, GL_FLOAT, faceData);
	}

	delete[] faceData;
}

void SkyBox::setupCubeMesh() {
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

void SkyBox::Bind(unsigned int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);
}

void SkyBox::Draw() {
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

SkyBox::~SkyBox() {
	if (skyBoxID) glDeleteTextures(1, &skyBoxID);
	if (skyboxVAO) glDeleteVertexArrays(1, &skyboxVAO);
	if (skyboxVBO) glDeleteBuffers(1, &skyboxVBO);
}