#include "IndexBuffer.hpp"

#include "stdlib.h"
#include "string.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &this->EBO_ID);

	this->size = 0;
	this->data = nullptr;
}

IndexBuffer::IndexBuffer(int size, const unsigned int *data)
{
	glGenBuffers(1, &this->EBO_ID);
	this->AddData(size, data);
}

IndexBuffer::~IndexBuffer()
{
	free(this->data);
}

void IndexBuffer::AddData(int size, const unsigned int *data)
{
	this->size = size;
	this->data = (unsigned int *)malloc(sizeof(unsigned int) * size);

	if (!this->data)
	{
		return;
	}

	memcpy(this->data, data, sizeof(unsigned int) * size);

	this->Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_ID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int IndexBuffer::GetSize()
{
	return this->size;
}