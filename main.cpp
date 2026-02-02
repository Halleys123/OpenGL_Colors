#include <stdio.h>
#include <Windows.h>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "WndProc.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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

static unsigned int index[24] = {
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

extern HDC hdc;
extern HGLRC rc;

static float yaw = -45.0f;
static float pitch = 0.0f;

static glm::vec3 curCamPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 curCamLookAt = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::vec3 cameraFront = glm::normalize(curCamLookAt - curCamPos);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static glm::vec3 directionalLightPosition = glm::vec3(-1.0f, 3.0f, 1.5f);
static glm::vec3 directionalLightColor = glm::vec3(1.0f, 0.5f, 0.5f);

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

	Mesh cube(24 * 11, data, 36, index);
	cube.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0);
	cube.AddVertexAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(3 * sizeof(float)));
	cube.AddVertexAttrib(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(5 * sizeof(float)));
	cube.AddVertexAttrib(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(8 * sizeof(float)));

	Shader shader("./shaders/One.glsl");
	Texture wood("./Textures/texture.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);
	//Texture ground("./Textures/ground_color.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);
	//Texture groundNormal("./Textures/ground_normal.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);

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
	glm::mat3 mv = glm::transpose(glm::inverse(glm::mat3(model)));

	wood.activate(0);
	//ground.activate(0);
	//groundNormal.activate(1);

	shader.UseProgram();

	shader.ModifyUniformVec3("ambientLightColor", 0.2f, 0.2f, 0.2f);
	shader.ModifyUniformVec3("directionalLightPosition", directionalLightPosition.x, directionalLightPosition.y, directionalLightPosition.z);
	shader.ModifyUniformVec3("directionalLightColor", 1.0f, 0.5f, 0.0f);
	shader.ModifyUniform1i("tex", 0);

	//shader.ModifyUniform1i("ground", 0);
	//shader.ModifyUniform1i("ground_normal", 1);

	shader.ModifyUniformMat3("mv", mv);

	float lightCube[8 * 3] = {
		// Front Side
		directionalLightPosition.x - 0.15f, directionalLightPosition.y - 0.15f, directionalLightPosition.z + 0.15f,  // 0: front-bottom-left
		directionalLightPosition.x + 0.15f, directionalLightPosition.y - 0.15f, directionalLightPosition.z + 0.15f,  // 1: front-bottom-right
		directionalLightPosition.x - 0.15f, directionalLightPosition.y + 0.15f, directionalLightPosition.z + 0.15f,  // 2: front-top-left
		directionalLightPosition.x + 0.15f, directionalLightPosition.y + 0.15f, directionalLightPosition.z + 0.15f,  // 3: front-top-right

		// Back Side
		directionalLightPosition.x - 0.15f, directionalLightPosition.y - 0.15f, directionalLightPosition.z - 0.15f,  // 4: back-bottom-left
		directionalLightPosition.x + 0.15f, directionalLightPosition.y - 0.15f, directionalLightPosition.z - 0.15f,  // 5: back-bottom-right
		directionalLightPosition.x - 0.15f, directionalLightPosition.y + 0.15f, directionalLightPosition.z - 0.15f,  // 6: back-top-left
		directionalLightPosition.x + 0.15f, directionalLightPosition.y + 0.15f, directionalLightPosition.z - 0.15f,   // 7: back-top-right
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

	Mesh light(27, lightCube, 36, lightCubeIndex);
	light.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	Shader lightShader("./shaders/lightCube.glsl");
	lightShader.UseProgram();
	lightShader.ModifyUniformVec3("lightColor", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);

	float eyePosX = 0.0f;
	float eyePosY = 0.0f;
	float inc = 0.0005f;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	float speed = 2.5f;     
	float sensitivity = 0.1f;
	float acceleration = 10.0f;  
	float deceleration = 15.0f;  
	
	glm::vec3 velocity = glm::vec3(0.0f); 

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

		shader.ModifyUniformVec3("EyePosition", cameraFront.x, cameraFront.y, cameraFront.z);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw regular geometry
		shader.UseProgram();
		shader.ModifyUniformMat4("mvp", mvp);
		cube.Draw();

		lightShader.UseProgram();
		lightShader.ModifyUniformMat4("mvp", mvp);
		light.Draw();

		SwapBuffers(hdc);
	}

	return 0;
}