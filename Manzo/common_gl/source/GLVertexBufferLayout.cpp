#include "GLVertexBufferLayout.h"

GLVertexBufferLayout::GLVertexBufferLayout()
    : m_Stride(0)
{
    // Constructor that initializes the stride to zero
}

// Specialized version of the Push() function for adding elements of type float
void GLVertexBufferLayout::AddFloat(unsigned int count)
{
    Push(GL_FLOAT, count, GL_FALSE);
}

// Specialized version of the Push() function for adding elements of type unsigned int
void GLVertexBufferLayout::AddUnsignedInt(unsigned int count)
{
    Push(GL_UNSIGNED_INT, count, GL_FALSE);
}

// Specialized version of the Push() function for adding elements of type unsigned char
void GLVertexBufferLayout::AddUnsignedChar(unsigned int count)
{
    Push(GL_UNSIGNED_BYTE, count, GL_TRUE);
}


void GLVertexBufferLayout::Push(unsigned int type, unsigned int count, unsigned char normalized)
{
    unsigned int size_of_element = count * VertexBufferElement::GetSizeOfType(type);

    // Update the offset of the current element
    unsigned int offset = m_Stride;
    unsigned int sz = static_cast<unsigned int>(m_Elements.size());

    // Create a new element and add it to the layout
    m_Elements.push_back({ type, count, normalized, sz, size_of_element, offset, 0 });

    // Increase the stride by the size of the new element
    m_Stride += size_of_element;
}
