#include <stdio.h>
#include <Windows.h>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "WndProc.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

extern HDC hdc;
extern HGLRC rc;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR cmd, _In_ int nCmd)
{
	WNDCLASS windowClass = {0};

	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = "OpenGLColors";
	windowClass.lpfnWndProc = WndProc;
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClass(&windowClass))
	{
		MessageBox(NULL, "Unable to register window class", "Internal Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	HWND hWnd = CreateWindow(windowClass.lpszClassName, "OpenGL Colors", WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 720, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Unable to create Window", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	int running = true;
	MSG msg = {0};

	// Format: { Position (x,y,z), Normal (x,y,z), TexCoord (u,v) }
	float data[24 * 8] = {
		// Front face (Normal: 0, 0, 1)
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f, // Bottom-Left
		 0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f, // Bottom-Right
		 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f, // Top-Right
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f, // Top-Left

		// Right face (Normal: 1, 0, 0)
		 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // Bottom-Left
		 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // Bottom-Right
		 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // Top-Right
		 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // Top-Left

		 // Back face (Normal: 0, 0, -1)
		 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // Bottom-Left
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f, // Bottom-Right
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // Top-Right
		 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f, // Top-Left

		 // Left face (Normal: -1, 0, 0)
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f, // Bottom-Left
		-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, // Bottom-Right
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f, // Top-Right
		-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, // Top-Left

		// Top face (Normal: 0, 1, 0)
	   -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f, // Bottom-Left
		0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f, // Bottom-Right
		0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f, // Top-Right
	   -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f, // Top-Left

	   // Bottom face (Normal: 0, -1, 0)
	  -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f, // Bottom-Left
	   0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f, // Bottom-Right
	   0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f, // Top-Right
	  -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f  // Top-Left
	};

	unsigned int index[36] = {
		// Front
		 0,  1,  2,
		 2,  3,  0,

		 // Right
		  4,  5,  6,
		  6,  7,  4,

		  // Back
		   8,  9, 10,
		  10, 11,  8,

		  // Left
		  12, 13, 14,
		  14, 15, 12,

		  // Top
		  16, 17, 18,
		  18, 19, 16,

		  // Bottom
		  20, 21, 22,
		  22, 23, 20
	};


	Mesh cube(144 + 48, data, 36, index);
	cube.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	cube.AddVertexAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	cube.AddVertexAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

	Shader shader("./shaders/One.glsl");
	Shader shader2("./shaders/Two.glsl");

	const double targetFrameTime = 1.0 / 90.0;

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	LARGE_INTEGER last;
	QueryPerformanceCounter(&last);
	LARGE_INTEGER now;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 720.0f / 480.0f, 0.1f, 100.0f);

	glm::mat4 mvp = projection * view * model;

	shader.UseProgram();
	shader.ModifyUniformVec3("objectColor", 0.234f, 0.324f, 0.0f);
	shader.ModifyUniformVec3("lightColor", 0.1f, 0.54f, 0.23f);
	
	shader2.UseProgram();
	shader2.ModifyUniformVec3("objectColor", 0.234f, 0.324f, 0.0f);
	shader2.ModifyUniformVec3("lightColor", 0.1f, 0.54f, 0.23f);

	//shader.ModifyUniformMat4("model", model);
	//shader.ModifyUniformMat4("view", view);
	//shader.ModifyUniformMat4("projection", projection);

	float eyePosX = 0.0f;

	glEnable(GL_DEPTH_TEST);
	while (running)
	{
		if (msg.message == WM_QUIT)
		{
			running = false;
		}

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		QueryPerformanceCounter(&now);
		if (now.QuadPart - last.QuadPart < 90)
			continue;
		last = now;

		view = glm::lookAt(glm::vec3(3.0f * sin(eyePosX), 1.0f, 3.0f * cos(eyePosX)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		eyePosX += 0.0005f;
		mvp = projection * view * model;
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.UseProgram();
		shader.ModifyUniformMat4("mvp", mvp);
		cube.Draw();
		
		shader2.UseProgram();
		shader2.ModifyUniformMat4("mvp", mvp);
		cube.Draw();

		SwapBuffers(hdc);
	}

	return 0;
}