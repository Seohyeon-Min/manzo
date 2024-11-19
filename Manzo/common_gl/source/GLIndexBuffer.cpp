/******************************************************************************
* @filename GLIndexBuffer.cpp
* @author   Seohyeon Min
* @date     09/10/2024
* @brief    This file implements the GLIndexBuffer class, which manages
*           OpenGL index buffer objects (IBOs). It handles the creation,
*           deletion, and binding of index buffers with various data types.
******************************************************************************/

#include "GLIndexBuffer.h"

/******************************************************************************
* @brief    Constructor for GLIndexBuffer class that accepts unsigned char indices.
*           Initializes the index buffer with GLubyte type elements.
* @param    indices A span of unsigned char representing the indices.
******************************************************************************/
GLIndexBuffer::GLIndexBuffer(std::span<const unsigned char> indices)
    : element_type(GLIndexElement::UByte), count(static_cast<GLsizei>(indices.size()))
{
    create_indices_buffer(indices_handle, indices);              // Create buffer with unsigned char indices
}

/******************************************************************************
* @brief    Constructor for GLIndexBuffer class that accepts unsigned short indices.
*           Initializes the index buffer with GLushort type elements.
* @param    indices A span of unsigned short representing the indices.
******************************************************************************/
GLIndexBuffer::GLIndexBuffer(std::span<const unsigned short> indices)
    : element_type(GLIndexElement::UShort), count(static_cast<GLsizei>(indices.size()))
{
    create_indices_buffer(indices_handle, indices);              // Create buffer with unsigned short indices
}

/******************************************************************************
* @brief    Constructor for GLIndexBuffer class that accepts unsigned int indices.
*           Initializes the index buffer with GLuint type elements.
* @param    indices A span of unsigned int representing the indices.
******************************************************************************/
GLIndexBuffer::GLIndexBuffer(std::span<const unsigned int> indices)
    : element_type(GLIndexElement::UInt), count(static_cast<GLsizei>(indices.size()))
{
    create_indices_buffer(indices_handle, indices);              // Create buffer with unsigned int indices
}

/******************************************************************************
* @brief    Destructor for GLIndexBuffer class. Deletes the index buffer handle.
******************************************************************************/
GLIndexBuffer::~GLIndexBuffer() {
    if (indices_handle != 0) {
        glCheck(glDeleteBuffers(1, &indices_handle));            // Delete the buffer object
    }
}

/******************************************************************************
* @brief    Move constructor for GLIndexBuffer class.
* @param    temp Temporary GLIndexBuffer object to be moved from.
******************************************************************************/
GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& temp) noexcept
    : indices_handle(temp.indices_handle),
    element_type(temp.element_type),
    count(temp.count)
{
    temp.indices_handle = 0;                                     // Invalidate the handle in the source object
}

/******************************************************************************
* @brief    Move assignment operator for GLIndexBuffer class.
* @param    temp Temporary GLIndexBuffer object to be moved from.
* @return   Reference to the current object after assignment.
******************************************************************************/
GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& temp) noexcept
{
    if (this != &temp) {
        if (indices_handle != 0) {
            glCheck(glDeleteBuffers(1, &indices_handle));        // Delete the current buffer if it exists
        }

        indices_handle = temp.indices_handle;                    // Move the handle and other properties
        element_type = temp.element_type;
        count = temp.count;

        temp.indices_handle = 0;                                 // Invalidate the handle in the source object
    }

    return *this;
}

void GLIndexBuffer::CreateIndexBuffer(const unsigned int* indices, unsigned int count)
{
    m_count = count;

    // Generate a new buffer ID for the index buffer
    glGenBuffers(1, &indices_handle);

    // Bind the buffer to the GL_ELEMENT_ARRAY_BUFFER target
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_handle);

    // Copy the index data to the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

/******************************************************************************
* @brief    Binds or unbinds the index buffer based on the bind parameter.
* @param    bind If true, bind the buffer; otherwise unbind it.
******************************************************************************/
void GLIndexBuffer::Use(bool bind) const
{
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bind ? indices_handle : 0)); // Bind or unbind the index buffer
}
