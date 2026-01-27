#pragma once

class IndexBuffer {
private:
	unsigned int EBO_ID;
	int size;
	unsigned int* data;
public:
	IndexBuffer();
	IndexBuffer(int size, const unsigned int* data);
	~IndexBuffer();
	void AddData(int size, const unsigned int* data);
	void Bind();
	void Unbind();
	int GetSize();
};