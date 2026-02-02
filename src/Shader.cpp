#include <Windows.h>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader.hpp"
#include "LoadFile.hpp"

#include "stdio.h"
#include "stdlib.h"

static void loadShaders(const char *path, char **vertexShaderSrc, char **fragShaderSrc)
{
	char *content = nullptr;
	int fileSize = LoadFile(path, &content);

	if (!content || fileSize <= 0)
	{
		*vertexShaderSrc = nullptr;
		*fragShaderSrc = nullptr;
		return;
	}

	int index = 0;

	// Find '~'
	while (index < fileSize && content[index] != '~')
	{
		index++;
	}

	if (index == fileSize)
	{
		// No separator found
		free(content);
		*vertexShaderSrc = nullptr;
		*fragShaderSrc = nullptr;
		return;
	}

	int vertexSize = index;
	int fragmentSize = fileSize - vertexSize - 1;

	*vertexShaderSrc = (char *)malloc(vertexSize + 1);
	*fragShaderSrc = (char *)malloc(fragmentSize + 1);

	// Copy vertex shader
	memcpy(*vertexShaderSrc, content, vertexSize);
	(*vertexShaderSrc)[vertexSize] = '\0';

	// Copy fragment shader
	memcpy(*fragShaderSrc, content + vertexSize + 1, fragmentSize);
	(*fragShaderSrc)[fragmentSize] = '\0';

	free(content);
}

Shader::Shader(const char *ShaderPath)
{
	char *VertexShaderSrc, *FragShaderSrc;
	loadShaders(ShaderPath, &VertexShaderSrc, &FragShaderSrc);

	this->ShaderProgramID = glCreateProgram();

	this->VertexShaderID = CompileShader(VertexShaderSrc, GL_VERTEX_SHADER);
	this->FragShaderID = CompileShader(FragShaderSrc, GL_FRAGMENT_SHADER);

	glAttachShader(ShaderProgramID, VertexShaderID);
	glAttachShader(ShaderProgramID, FragShaderID);

	glLinkProgram(ShaderProgramID);
	glValidateProgram(ShaderProgramID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragShaderID);

	free(VertexShaderSrc);
	free(FragShaderSrc);
}

unsigned int Shader::CompileShader(char *ShaderSrc, GLenum ShaderType)
{
	unsigned int ShaderID = glCreateShader(ShaderType);
	glShaderSource(ShaderID, 1, &ShaderSrc, NULL);
	glCompileShader(ShaderID);

	int CompileStatus;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &CompileStatus);

	if (CompileStatus == GL_FALSE)
	{
		int msgLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &msgLength);
		char *message = (char *)malloc(sizeof(char) * msgLength);
		glGetShaderInfoLog(ShaderID, msgLength, &msgLength, message);
		MessageBox(NULL, message, "OpenGL Error", MB_OK | MB_ICONERROR);
		free(message);
		glDeleteShader(ShaderID);
		return 0;
	}

	return ShaderID;
}

void Shader::UseProgram()
{
	glUseProgram(this->ShaderProgramID);
}

void Shader::ModifyUniformMat4(const char *uniformName, glm::mat4 &matrix)
{
	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);
	if (location == -1)
	{
		printf("Unable to find Uniform named: %s", uniformName);
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
void Shader::ModifyUniformMat3(const char* uniformName, glm::mat3& matrix) {
	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);
	if (location == -1)
	{
		printf("Unable to find Uniform named: %s", uniformName);
		return;
	}

	glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);

}

void Shader::ModifyUniformVec3(const char* uniformName, float x, float y, float z) {
	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);
	if (location == -1)
	{
		printf("Unable to find Uniform named: %s", uniformName);
		return;
	}

	glUniform3f(location, x, y, z);
}

void Shader::ModifyUniform1i(const char* uniformName, int value) {
	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);
	if (location == -1)
	{
		printf("Unable to find Uniform named: %s", uniformName);
		return;
	}

	glUniform1i(location, value);
}

void Shader::ModifyUniform1f(const char* uniformName, float value) {
	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);
	if (location == -1)
	{
		printf("Unable to find Uniform named: %s", uniformName);
		return;
	}

	glUniform1f(location, value);
}
