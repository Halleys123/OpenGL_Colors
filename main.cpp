#include <stdio.h>
#include <Windows.h>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "WndProc.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "SkyBox.hpp"


static float data[18 * 5] = {
	// === Bottom triangle ===
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 0
	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 1
	0.5f, 0.0f, 1.0f,   0.5f, 1.0f, // 2

	// === Top triangle ===
	0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // 3
	1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 4
	0.5f, 1.0f, 1.0f,   0.5f, 1.0f, // 5

	// === Side 1 (left rectangle) ===
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 6
	0.5f, 0.0f, 1.0f,   1.0f, 0.0f, // 7
	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 8
	0.5f, 1.0f, 1.0f,   1.0f, 1.0f, // 9

	// === Side 2 (right rectangle) ===
	1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 10
	0.5f, 0.0f, 1.0f,   1.0f, 0.0f, // 11
	1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 12
	0.5f, 1.0f, 1.0f,   1.0f, 1.0f, // 13

	// === Side 3 (bottom rectangle) ===
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // 14
	1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // 15
	0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // 16
	1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // 17
};

static unsigned int index[24] = {
	// Bottom triangle
	0, 1, 2,

	// Top triangle
	3, 5, 4,

	// Side 1
	6, 7, 8,
	7, 9, 8,

	// Side 2
	10, 11, 12,
	11, 13, 12,

	// Side 3
	14, 15, 16,
	15, 17, 16
};

extern HDC hdc;
extern HGLRC rc;

static float yaw = -45.0f;
static float pitch = 0.0f;

static glm::vec3 curCamPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 curCamLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraFront = glm::normalize(curCamLookAt - curCamPos);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

	HWND hWnd = CreateWindowEx(0, windowClass.lpszClassName, "OpenGL Colors", WS_VISIBLE | WS_POPUP, 0, 0, 1440, 960, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Unable to create Window", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	ShowCursor(false);

	int running = true;
	MSG msg = {0};

	Mesh cube(90, data, 24, index);
	cube.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	//cube.AddVertexAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	cube.AddVertexAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	Shader shader("./shaders/One.glsl");
	//Shader skyboxShader("./shaders/Skybox.glsl");

	Texture wood("./Textures/roof.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);
	//Texture normalMap("./Textures/height.png", GL_TEXTURE_2D);

	//SkyBox box("./Textures/skyBox.hdr");

	const double targetFrameTime = 1.0 / 90.0;

	int maxUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
		printf("Max Texture Units available: %d\n", maxUnits);

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	LARGE_INTEGER last;
	QueryPerformanceCounter(&last);
	LARGE_INTEGER now;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.5f, 0.5f, 0.333f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 720.0f / 480.0f, 0.1f, 100.0f);

	glm::mat4 mvp = projection * view * model;

	wood.activate(0);
	//normalMap.activate(1);
	shader.UseProgram();

	shader.ModifyUniformVec3("objectColor", 0.234f, 0.324f, 0.0f);
	shader.ModifyUniformVec3("lightColor", 0.1f, 0.54f, 0.23f);

	shader.ModifyUniform1i("tex", 0);
	//shader.ModifyUniform1i("hMap", 1);

	// Setup skybox shader
	//skyboxShader.UseProgram();
	//skyboxShader.ModifyUniform1i("skybox", 0);
	//skyboxShader.ModifyUniform1f("exposure", 1.0f);

	float eyePosX = 0.0f;
	float eyePosY = 0.0f;
	float inc = 0.0005f;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	float speed = 2.5f;     // Max speed in units per second
	float sensitivity = 0.1f;
	float acceleration = 10.0f;  // Units per second squared
	float deceleration = 15.0f;  // Units per second squared (faster stop)
	
	glm::vec3 velocity = glm::vec3(0.0f); // Current velocity vector

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
		deltaTime = (float)(now.QuadPart - last.QuadPart) / freq.QuadPart;
		last = now;

		if (GetForegroundWindow() == hWnd)
		{
			POINT curMouse;
			GetCursorPos(&curMouse);

			RECT rect;
			GetClientRect(hWnd, &rect);
			POINT windowCenter = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };

			ClientToScreen(hWnd, &windowCenter);

			float xOffset = (float)(curMouse.x - windowCenter.x);
			float yOffset = (float)(windowCenter.y - curMouse.y); // Reversed Y

			yaw += xOffset * sensitivity;
			pitch += yOffset * sensitivity;

			if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;

			SetCursorPos(windowCenter.x, windowCenter.y);
		}

		// --- 4. SMOOTH KEYBOARD MOVEMENT WITH ACCELERATION ---
		glm::vec3 inputDir = glm::vec3(0.0f);
		
		if (GetAsyncKeyState('W') & 0x8000)
			inputDir += cameraFront;

		if (GetAsyncKeyState('S') & 0x8000)
			inputDir -= cameraFront;

		if (GetAsyncKeyState('A') & 0x8000)
			inputDir -= glm::normalize(glm::cross(cameraFront, cameraUp));

		if (GetAsyncKeyState('D') & 0x8000)
			inputDir += glm::normalize(glm::cross(cameraFront, cameraUp));

		// Escape to exit
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			running = false;

		// Apply acceleration or deceleration
		if (glm::length(inputDir) > 0.0f)
		{
			// Normalize input direction and accelerate
			inputDir = glm::normalize(inputDir);
			velocity += inputDir * acceleration * deltaTime;
			
			// Clamp velocity to max speed
			if (glm::length(velocity) > speed)
				velocity = glm::normalize(velocity) * speed;
		}
		else
		{
			// Decelerate when no input
			float currentSpeed = glm::length(velocity);
			if (currentSpeed > 0.0f)
			{
				float newSpeed = currentSpeed - deceleration * deltaTime;
				if (newSpeed < 0.0f)
					velocity = glm::vec3(0.0f);
				else
					velocity = glm::normalize(velocity) * newSpeed;
			}
		}

		// Apply velocity to camera position
		curCamPos += velocity * deltaTime;

		// --- 5. Update Camera Vectors ---
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		// --- 6. Render ---
		view = glm::lookAt(curCamPos, curCamPos + cameraFront, cameraUp);
		mvp = projection * view * model;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw skybox first (with special depth settings handled internally)
		glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Remove translation from view matrix
		//skyboxShader.UseProgram();
		//skyboxShader.ModifyUniformMat4("view", skyboxView);
		//skyboxShader.ModifyUniformMat4("projection", projection);
		//box.Draw();

		// Draw regular geometry
		shader.UseProgram();
		shader.ModifyUniformMat4("mvp", mvp);
		cube.Draw();

		SwapBuffers(hdc);
	}

	return 0;
}