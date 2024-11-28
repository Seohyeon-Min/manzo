#pragma once
#include "GLVertexBuffer.h"
#include "GLVertexArray.h"
#include "GLIndexbuffer.h"
#include "GLVertexBufferLayout.h"
#include "GLTexture.h"

class gBuffers
{
public:
	gBuffers();
	~gBuffers();
	void Bind()const;
	void UnBind()const;
	void CreateBuffers(const void* vb_data, 
		unsigned int& vb_size, 
		const unsigned int* ib_indices, 
		unsigned int& ib_count, 
		const GLVertexBufferLayout& layout);
	GLVertexBuffer vbo;
	GLVertexArray vao;
	GLIndexBuffer ibo;
private:
	unsigned int vbo_sz;
};
