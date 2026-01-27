#include "VertexBuffer.hpp"

#include "stdlib.h"
#include "string.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &this->VBO_ID);

	this->size = 0;
	this->data = nullptr;
}

VertexBuffer::VertexBuffer(int size, const float *data)
{
	glGenBuffers(1, &this->VBO_ID);
	this->Bind();
	this->AddData(size, data);
}

VertexBuffer::~VertexBuffer()
{
	free(this->data);
}

void VertexBuffer::AddData(int size, const float *data)
{
	if (this->data)
	{
		free(this->data);
	}

	this->size = size;
	this->data = (float *)malloc(sizeof(float) * size);

	memcpy(this->data, data, sizeof(float) * size);

	this->Bind();
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_ID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int VertexBuffer::GetSize()
{
	return this->size;
}