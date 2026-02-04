#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Camera settings
glm::vec3 curCamPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 curCamLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::normalize(curCamLookAt - curCamPos);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Light settings
glm::vec3 directionalLightPosition = glm::vec3(-1.0f, 0.5f, -0.0f);
glm::vec3 directionalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

// Camera setting and MVP matrices
glm::mat4 model_matrix = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.5f, 0.5f, 0.333f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 projection = glm::perspective(glm::radians(60.0f), 720.0f / 480.0f, 0.1f, 100.0f);

glm::mat4 mvp = projection * view * model_matrix;
glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));

// Camera Roation
float yaw = -45.0f;
float pitch = 0.0f;

// Camera Movement Velocity
glm::vec3 velocity = glm::vec3(0.0f);

float data[18 * 11] = {
	// Front triangle
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	0.0f,  0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,

	// Back triangle
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	0.0f,  0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,

	// Left side vertices - CORRECTED NORMALS
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.447f, 0.894f, 0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  -0.447f, 0.894f, 0.0f,
	0.0f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  -0.447f, 0.894f, 0.0f,
	0.0f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  -0.447f, 0.894f, 0.0f,

	// Right side vertices - CORRECTED NORMALS
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.447f, 0.894f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.447f, 0.894f, 0.0f,
	0.0f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.447f, 0.894f, 0.0f,
	0.0f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.447f, 0.894f, 0.0f,

	// Bottom side
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f
};

float surface_mesh_coords[20] = {
	// Vertex 0: Bottom-left
	-40.0f, -0.001f, -20.0f,   // Position
	0.0f, 0.0f,				// UV

	// Vertex 1: Top-left  
	-40.0f, -0.001f, 60.0f,   // Position
	0.0f, 40.0f,				// UV

	// Vertex 2: Bottom-right
	80.0f, -0.001f, -20.0f,   // Position
	40.0f, 0.0f,				// UV

	// Vertex 3: Top-right
	80.0f, -0.001f, 60.0f,   // Position
	40.0f, 40.0f,				// UV

};

unsigned int surface_mesh_index[6] = {
	// First triangle
	0, 1, 2,
	// Second triangle
	1, 2, 3
};

unsigned int index[24] = {
	// Front triangle
	0, 1, 2,

	// Back triangle
	3, 5, 4,

	// Left side (2 triangles)
	6, 7, 8,
	6, 8, 9,

	// Right side (2 triangles)
	10, 12, 11,
	10, 13, 12,

	// Bottom side (2 triangles)
	14, 15, 16,
	14, 16, 17
};


float lightCube[8 * 3] = {
	// Front Side
	directionalLightPosition.x - 0.01f, directionalLightPosition.y - 0.01f, directionalLightPosition.z + 0.01f,  // 0: front-bottom-left
	directionalLightPosition.x + 0.01f, directionalLightPosition.y - 0.01f, directionalLightPosition.z + 0.01f,  // 1: front-bottom-right
	directionalLightPosition.x - 0.01f, directionalLightPosition.y + 0.01f, directionalLightPosition.z + 0.01f,  // 2: front-top-left
	directionalLightPosition.x + 0.01f, directionalLightPosition.y + 0.01f, directionalLightPosition.z + 0.01f,  // 3: front-top-right

	// Back Side
	directionalLightPosition.x - 0.01f, directionalLightPosition.y - 0.01f, directionalLightPosition.z - 0.01f,  // 4: back-bottom-left
	directionalLightPosition.x + 0.01f, directionalLightPosition.y - 0.01f, directionalLightPosition.z - 0.01f,  // 5: back-bottom-right
	directionalLightPosition.x - 0.01f, directionalLightPosition.y + 0.01f, directionalLightPosition.z - 0.01f,  // 6: back-top-left
	directionalLightPosition.x + 0.01f, directionalLightPosition.y + 0.01f, directionalLightPosition.z - 0.01f,   // 7: back-top-right
};

unsigned int lightCubeIndex[36] = {
	// Front face
	0, 1, 2,
	1, 3, 2,

	// Back face
	4, 6, 5,
	5, 6, 7,

	// Left face
	4, 0, 6,
	0, 2, 6,

	// Right face
	1, 5, 3,
	5, 7, 3,

	// Top face
	2, 3, 6,
	3, 7, 6,

	// Bottom face
	0, 4, 1,
	4, 5, 1
};
