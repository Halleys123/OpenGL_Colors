#include <stdio.h>
#include <Windows.h>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "WndProc.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "Properties.hpp"

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

	Mesh light(27, lightCube, 36, lightCubeIndex);
	light.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	Mesh surface(32, surface_mesh_coords, 6, surface_mesh_index);
	surface.AddVertexAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	surface.AddVertexAttrib(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	Shader lightShader("./shaders/lightCube.glsl");
	Shader shader("./shaders/One.glsl");
	Shader surfaceShader("./shaders/Surface.glsl");

	Texture wood("./Textures/texture.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);
	Texture ground("./Textures/ground_color.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);
	Texture ground_normal("./Textures/ground_normal.jpg", GL_TEXTURE_2D, GL_RGB8, GL_RGB);

	wood.activate(0);
	ground.activate(1);
	ground_normal.activate(2);

	shader.UseProgram();
	shader.ModifyUniformVec3("ambientLightColor", 0.2f, 0.2f, 0.2f);
	shader.ModifyUniformVec3("directionalLightPosition", directionalLightPosition.x, directionalLightPosition.y, directionalLightPosition.z);
	shader.ModifyUniformVec3("directionalLightColor", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
	shader.ModifyUniform1i("tex", 0);
	shader.ModifyUniformMat4("model_matrix", model_matrix);
	shader.ModifyUniformMat3("normal_matrix", normal_matrix);

	surfaceShader.UseProgram();
	surfaceShader.ModifyUniformVec3("ambientLightColor", 0.2f, 0.2f, 0.2f);
	surfaceShader.ModifyUniformVec3("directionalLightPosition", directionalLightPosition.x, directionalLightPosition.y, directionalLightPosition.z);
	surfaceShader.ModifyUniformVec3("directionalLightColor", directionalLightColor.x, directionalLightColor.y, directionalLightColor.z);
	surfaceShader.ModifyUniformMat4("model_matrix", model_matrix);
	surfaceShader.ModifyUniformMat3("normal_matrix", normal_matrix);
	surfaceShader.ModifyUniform1i("ground", 1);
	surfaceShader.ModifyUniform1i("ground_normal", 2);

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

	const double targetFrameTime = 1.0 / 90.0;

	int maxUnits;
	
	LARGE_INTEGER freq, last, now;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&last);

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

		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
				pitch += 0.05f;
			else 
				inputDir += glm::normalize(cameraUp);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
				pitch -= 0.05f;
			else
				inputDir -= glm::normalize(cameraUp);
		}
		
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			yaw -= 0.05f;

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			yaw += 0.05f;

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
		mvp = projection * view * model_matrix;


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw regular geometry
		shader.UseProgram();
		shader.ModifyUniformMat4("mvp", mvp);
		shader.ModifyUniformVec3("EyePosition", curCamPos.x, curCamPos.y, curCamPos.z);
		cube.Draw();

		lightShader.UseProgram();
		lightShader.ModifyUniformMat4("mvp", mvp);
		light.Draw();

		surfaceShader.UseProgram();
		surfaceShader.ModifyUniformMat4("mvp", mvp);
		surfaceShader.ModifyUniformVec3("EyePosition", curCamPos.x, curCamPos.y, curCamPos.z);
		surface.Draw();

		SwapBuffers(hdc);
	}

	return 0;
}