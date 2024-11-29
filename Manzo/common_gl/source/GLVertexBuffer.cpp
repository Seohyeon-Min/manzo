/******************************************************************************
* @filename GLVertexBuffer.cpp
* @author   Seohyeon Min
* @date     09/10/2024
* @brief    This file implements the GLVertexBuffer class, which manages
*           OpenGL buffer objects (VBOs). It handles the creation, deletion,
*           binding, and uploading of buffer data to the GPU.
******************************************************************************/

#include "GLVertexBuffer.h"
#include "glCheck.h"

/******************************************************************************
* @brief    Constructor for GLVertexBuffer class. Allocates a buffer with the
*           specified size in bytes and prepares it for dynamic storage.
* @param    size_in_bytes The size of the buffer in bytes.
******************************************************************************/
GLVertexBuffer::GLVertexBuffer(GLsizei size_in_bytes)
    : size(size_in_bytes)
{
    glCheck(glCreateBuffers(1, &buffer_handle));                 // Create the buffer object
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, buffer_handle));                // Bind it as an array buffer
    glCheck(glNamedBufferStorage(buffer_handle, size_in_bytes, nullptr, GL_DYNAMIC_STORAGE_BIT)); // Allocate dynamic storage
}

/******************************************************************************
* @brief    Destructor for GLVertexBuffer class. Deletes the buffer handle.
******************************************************************************/
GLVertexBuffer::~GLVertexBuffer()
{
    glCheck(glDeleteBuffers(1, &buffer_handle));                 // Delete the buffer object
}

/******************************************************************************
* @brief    Move constructor for GLVertexBuffer class.
* @param    temp Temporary GLVertexBuffer object to be moved from.
******************************************************************************/
GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& temp) noexcept
    : buffer_handle(temp.buffer_handle)
{
    temp.buffer_handle = 0;                                      // Invalidate the handle in the source object
}

/******************************************************************************
* @brief    Move assignment operator for GLVertexBuffer class.
* @param    temp Temporary GLVertexBuffer object to be moved from.
* @return   Reference to the current object after assignment.
******************************************************************************/
GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& temp) noexcept
{
    if (this != &temp) {
        if (buffer_handle != 0) {
            glCheck(glDeleteBuffers(1, &buffer_handle));         // Delete the current buffer if it exists
        }

        buffer_handle = temp.buffer_handle;                      // Move the handle and size
        size = temp.size;

        temp.buffer_handle = 0;                                  // Invalidate the handle in the source object
    }

    return *this;
}

/******************************************************************************
* @brief    Binds or unbinds the buffer based on the bind parameter.
* @param    bind If true, bind the buffer; otherwise unbind it.
******************************************************************************/
void GLVertexBuffer::Use(bool bind) const
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, bind ? buffer_handle : 0)); // Bind or unbind the buffer
}

void GLVertexBuffer::CreateVertexBuffer(const void* data, unsigned int size)
{
    // Main Constructor
    glGenBuffers(1, &buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}


/******************************************************************************
* @brief    Sends data from CPU memory to the GPU for this buffer.
* @param    data Pointer to the data to send.
* @param    size_bytes The size of the data in bytes.
* @param    starting_offset The offset in the buffer where the data will be placed.
******************************************************************************/
void GLVertexBuffer::send_buffer_data_to_gpu(const void* data, GLsizei size_bytes, GLsizei starting_offset) const
{
    glCheck(glNamedBufferSubData(buffer_handle, starting_offset, size_bytes, data)); // Upload the data to the GPU buffer
}
