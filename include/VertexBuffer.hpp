#pragma once

#include "glad/glad.h"

class VertexBuffer {
private:
	unsigned int VBO_ID;
	int size;
	float* data;
public:
	VertexBuffer();
	VertexBuffer(int size, const float* data);
	~VertexBuffer();
	void AddData(int size, const float* data);
	void Bind();
	void Unbind();
	int GetSize();
};