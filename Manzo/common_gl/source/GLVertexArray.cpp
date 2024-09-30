/******************************************************************************
* @filename GLVertexArray.cpp
* @author   Seohyeon Min
* @date     09/10/2024
* @brief    This file implements the GLVertexArray class, which encapsulates
*           OpenGL vertex array object (VAO) management. It provides functionalities
*           for adding vertex and index buffers, and for drawing indexed or non-indexed primitives.
******************************************************************************/

#include "GLVertexArray.h"
#include "glCheck.h"

/******************************************************************************
* @brief    Constructor for the GLVertexArray class.
* @param    the_primitive_pattern The primitive type (e.g., GL_TRIANGLES).
******************************************************************************/
GLVertexArray::GLVertexArray(GLPrimitive::Type the_primitive_pattern)
    : primitive_pattern(the_primitive_pattern)
{
    glCheck(glCreateVertexArrays(1, &vertex_array_handle)); // Create the vertex array object
}

/******************************************************************************
* @brief    Destructor for the GLVertexArray class.
*           Deletes the vertex array object if it exists.
******************************************************************************/
GLVertexArray::~GLVertexArray()
{
    if (vertex_array_handle != 0) {
        glCheck(glDeleteVertexArrays(1, &vertex_array_handle)); // Delete the VAO
    }
}

/******************************************************************************
* @brief    Move constructor for GLVertexArray class.
* @param    temp Temporary GLVertexArray object to be moved from.
******************************************************************************/
GLVertexArray::GLVertexArray(GLVertexArray&& temp) noexcept
    : vertex_array_handle(temp.vertex_array_handle),
    vertex_buffers(std::move(temp.vertex_buffers)),
    index_buffer(std::move(temp.index_buffer)),
    num_indices(temp.num_indices),
    indices_type(temp.indices_type),
    primitive_pattern(temp.primitive_pattern),
    num_vertices(temp.num_vertices)
{
    temp.vertex_array_handle = 0; // Invalidate the handle in the source object
}

/******************************************************************************
* @brief    Move assignment operator for GLVertexArray class.
* @param    temp Temporary GLVertexArray object to be moved from.
* @return   Reference to the current object after assignment.
******************************************************************************/
GLVertexArray& GLVertexArray::operator=(GLVertexArray&& temp) noexcept
{
    if (this != &temp) {
        // Swap the values with the temp object
        std::swap(vertex_array_handle, temp.vertex_array_handle);
        std::swap(vertex_buffers, temp.vertex_buffers);
        std::swap(index_buffer, temp.index_buffer);
        std::swap(num_indices, temp.num_indices);
        std::swap(indices_type, temp.indices_type);
        std::swap(primitive_pattern, temp.primitive_pattern);
        std::swap(num_vertices, temp.num_vertices);
    }
    return *this;
}

/******************************************************************************
* @brief    Binds or unbinds the vertex array object based on the bind parameter.
* @param    bind If true, bind the VAO, otherwise unbind it.
******************************************************************************/
void GLVertexArray::Use(bool bind) const
{
    if (bind) {
        glCheck(glBindVertexArray(vertex_array_handle)); // Bind the VAO
    }
    else {
        glCheck(glBindVertexArray(0)); // Unbind the VAO
    }
}

/******************************************************************************
* @brief    Adds a vertex buffer to the VAO and configures the buffer layout.
* @param    vertex_buffer The vertex buffer to add.
* @param    buffer_layout Initializer list of attributes for the buffer layout.
******************************************************************************/
void GLVertexArray::AddVertexBuffer(GLVertexBuffer&& vertex_buffer, std::initializer_list<GLAttributeLayout> buffer_layout)
{
    for (const auto& attribute : buffer_layout) // Configure each attribute in the buffer layout
    {
        glCheck(glEnableVertexArrayAttrib(vertex_array_handle, attribute.vertex_layout_location));
        glCheck(glVertexArrayVertexBuffer(vertex_array_handle, attribute.vertex_layout_location, vertex_buffer.GetHandle(), attribute.offset, attribute.stride));
        glCheck(glVertexArrayAttribFormat(vertex_array_handle, attribute.vertex_layout_location, attribute.component_dimension, attribute.component_type, attribute.normalized, attribute.relative_offset));
        glCheck(glVertexArrayAttribBinding(vertex_array_handle, attribute.vertex_layout_location, attribute.vertex_layout_location));
    }

    vertex_buffers.push_back(std::move(vertex_buffer)); // Store the vertex buffer
}

/******************************************************************************
* @brief    Sets the index buffer for the VAO.
* @param    the_indices The index buffer to set.
******************************************************************************/
void GLVertexArray::SetIndexBuffer(GLIndexBuffer&& the_indices)
{
    num_indices = the_indices.GetCount(); // Store the number of indices
    indices_type = the_indices.GetElementType(); // Store the type of indices

    glCheck(glVertexArrayElementBuffer(vertex_array_handle, the_indices.GetHandle())); // Set the index buffer
    this->index_buffer = std::move(the_indices); // Store the index buffer
}

/******************************************************************************
* @brief    Draws the elements in the VAO using indexed drawing.
* @param    vertex_array The vertex array to be drawn.
******************************************************************************/
void GLDrawIndexed(const GLVertexArray& vertex_array) noexcept
{
    glCheck(glDrawElements(vertex_array.GetPrimitivePattern(), vertex_array.GetIndicesCount(), vertex_array.GetIndicesType(), nullptr)); // Draw elements
}

/******************************************************************************
* @brief    Draws the vertices in the VAO without using indices.
* @param    vertex_array The vertex array to be drawn.
******************************************************************************/
void GLDrawVertices(const GLVertexArray& vertex_array) noexcept
{
    glCheck(glDrawArrays(vertex_array.GetPrimitivePattern(), 0, vertex_array.GetVertexCount())); // Draw arrays
}
