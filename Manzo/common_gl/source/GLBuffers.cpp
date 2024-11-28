#include "GLBuffers.h"

gBuffers::gBuffers()
	:vbo(1),vao(),ibo()
{
	// Empty constructor
}

gBuffers::~gBuffers()
{
	// Destructor
}

void gBuffers::CreateBuffers(const void* vb_data,
	unsigned int& vb_size,
	const unsigned int* ib_indices,
	unsigned int& ib_count,
	const GLVertexBufferLayout& vb_layout)
{
    // Check if data is valid
    if (!vb_data || vb_size == 0 || !ib_indices || ib_count == 0)
    {
        throw std::runtime_error("Invalid vertex or index data");
    }

    vao.CreateVertexArray();

    // Vertex Buffer Creation
    vbo_sz = vb_size;

	vbo.CreateVertexBuffer(vb_data, vb_size);

	// Index buffer (indices and number of indices)
	ibo.CreateIndexBuffer(ib_indices, ib_count);

	// Vertex Array (vertex buffer and vertex buffer layout) 
	vao.AddBuffer(vbo, vb_layout);
}

void gBuffers::Bind() const
{
	// Bind the buffers
	vao.Use(true);
	ibo.Use(true);
}


void gBuffers::UnBind() const
{
	// Un Bind the buffers
	vao.Use(false);
	ibo.Use(false);
}
