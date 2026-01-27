#include "Mesh.hpp"

Mesh::Mesh(int vertex_size, const float *vertex_data, int index_size, const unsigned int *index_data)
{
	VAO.Bind();
	VBO.AddData(vertex_size, vertex_data);
	IBO.AddData(index_size, index_data);
	VAO.Unbind();
}

void Mesh::AddVertexAttrib(unsigned int index, int size, GLenum type, GLboolean normalized, int stride, const void *pointer)
{
	VAO.Bind();
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	VAO.Unbind();
}

void Mesh::Draw(GLenum type)
{
	VAO.Bind();
	glDrawElements(type, IBO.GetSize(), GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}