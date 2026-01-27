#pragma once

#include "glad/glad.h"

class VertexArray {
private:
	unsigned int VAO_ID;
public:
	VertexArray();
	void AddVertexAttrib(unsigned int index, int size, GLenum type, GLboolean normalized, int stride, const void* pointer);
	void Bind();
	void Unbind();
};