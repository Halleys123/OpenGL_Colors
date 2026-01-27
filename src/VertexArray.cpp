#include "VertexArray.hpp"

#include "glad/glad.h"
#include "GlHelp.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &this->VAO_ID));
}

void VertexArray::Bind()
{
	GlCall(glBindVertexArray(this->VAO_ID));
}

void VertexArray::Unbind()
{
	GlCall(glBindVertexArray(0));
}


void VertexArray::AddVertexAttrib(unsigned int index, int size, GLenum type, GLboolean normalized, int stride, const void* pointer)
{
	this->Bind();
	GlCall(glEnableVertexAttribArray(index));
	GlCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
	this->Unbind();
}
