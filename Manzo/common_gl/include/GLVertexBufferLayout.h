#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <assert.h>
// Thanks to The Cherno

// This struct defines a single element in the vertex buffer layout
struct  VertexBufferElement
{
public:

	unsigned int type;          // The data type of the element (e.g. GL_FLOAT)
	unsigned int count;         // The number of elements of this type (e.g. 3 for a 3-component vector)
	unsigned char normalized;   // Whether the dstruct VertexBufferLayout::VertexBufferElement
	unsigned int location;      // Location of the attribute in the vertex shader
	unsigned int stride;        // Stride for the element (distance between consecutive elements in bytes)
	unsigned int offset;        // Offset for the element in the buffer
	unsigned int relative_offset; // Relative offset (if needed for certain layouts)

	VertexBufferElement(unsigned int t, unsigned int c, unsigned char n, unsigned int loc = 0, unsigned int str = 0, unsigned int off = 0, unsigned int rel_off = 0)
		: type(t), count(c), normalized(n), location(loc), stride(str), offset(off), relative_offset(rel_off) {}

	static unsigned int GetSizeOfType(unsigned int type)
	{
		//This function returns the size of a single element of this type in bytes
		switch (type)
		{
		case GL_FLOAT:          return 4;
		case GL_UNSIGNED_INT:   return 4;
		case GL_UNSIGNED_BYTE:  return 1;
		}
		assert(false);
		return 0;
	}
private:

};

class GLVertexBufferLayout
{
public:
	GLVertexBufferLayout();

	// Specialized version of the Push() function for adding elements of type float
	void AddFloat(unsigned int count);

	// Specialized version of the Push() function for adding elements of type unsigned int
	void AddUnsignedInt(unsigned int count);

	// Specialized version of the Push() function for adding elements of type unsigned char
	void AddUnsignedChar(unsigned int count);

	// Getter function that returns the vector of vertex buffer elements
	inline const  std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	// Getter function that returns the stride of the vertex buffer layout in bytes
	inline  unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;  // The vector of vertex buffer elements
	unsigned int m_Stride;                        // The stride of the vertex buffer layout in bytes
	void Push(unsigned int type, unsigned int count, unsigned char normalized);
};