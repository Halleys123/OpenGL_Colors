#pragma once

#include "glad/glad.h"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Mesh {
private:
	VertexArray VAO;
	VertexBuffer VBO;
	IndexBuffer IBO;
public:
	Mesh(int vertex_size, const float* vertex_data, int index_size, const unsigned int* index_data);
	void AddVertexAttrib(unsigned int index, int size, GLenum type, GLboolean normalized, int stride, const void* pointer);
	void Draw(GLenum type = GL_TRIANGLES);
};