#include <assert.h>
#include "Buffer.h"

//======================================================================================================
void Buffer::SetLineWidth(GLfloat lineWidth)
{
	glLineWidth(lineWidth);
}
//======================================================================================================
Buffer::Buffer(GLsizei totalVertices, bool hasEBO) : hasEBO(hasEBO), totalVertices(totalVertices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(4, VBOs);

	if (hasEBO)
	{
		glGenBuffers(1, &EBO);
	}
}
//======================================================================================================
Buffer::~Buffer()
{
	if (hasEBO)
	{
		glDeleteBuffers(1, &EBO);
	}

	glDeleteBuffers(4, VBOs);
	glDeleteVertexArrays(1, &VAO);
}
//======================================================================================================
void Buffer::FillEBO(const GLuint* data, GLsizeiptr bufferSize, Fill fill)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::FillVBO(VBO vbo, const void* data, GLsizeiptr bufferSize, Fill fill)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}
//======================================================================================================
void Buffer::AppendEBO(const GLuint* data, GLsizeiptr size, GLuint offset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::AppendVBO(VBO vbo, const void* data, GLsizeiptr size, GLuint offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
//======================================================================================================
void Buffer::LinkEBO()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::LinkVBO(GLint attributeID, VBO vbo, ComponentSize componentSize, DataType dataType)
{
	assert(attributeID > -1);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[static_cast<int>(vbo)]);

	glVertexAttribPointer(attributeID, static_cast<GLint>(componentSize),
		static_cast<GLenum>(dataType), GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(attributeID);
	glBindVertexArray(0);
}
//======================================================================================================
void Buffer::Render(RenderMode renderMode, GLuint index, GLuint totalVertices)
{
	glBindVertexArray(VAO);

	if (hasEBO)
	{
		if (index > 0)
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				totalVertices > 0 ? totalVertices : this->totalVertices,
				GL_UNSIGNED_INT, (const void*)(index));
		}

		else
		{
			glDrawElements(static_cast<GLenum>(renderMode),
				this->totalVertices, GL_UNSIGNED_INT, (const void*)(nullptr));
		}
	}

	else
	{
		glDrawArrays(static_cast<GLenum>(renderMode), 0, this->totalVertices);
	}

	glBindVertexArray(0);
}