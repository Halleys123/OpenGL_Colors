#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader {
private:	
	unsigned int ShaderProgramID;
	unsigned int VertexShaderID;
	unsigned int FragShaderID;
private:
	unsigned int CompileShader(char* ShaderSrc, GLenum ShaderType);
public:
	Shader(const char* ShaderPath);
	void UseProgram();
	void ModifyUniformMat4(const char* uniformName, glm::mat4& matrix);
	void ModifyUniformVec3(const char* uniformName, float x, float y, float z);
};