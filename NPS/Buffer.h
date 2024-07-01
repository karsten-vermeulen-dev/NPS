#pragma once
#include "GLAD/gl.h"

class Buffer
{

public:

	enum class Fill
	{
		Once = GL_STATIC_DRAW,
		Ongoing = GL_DYNAMIC_DRAW
	};

	enum class ComponentSize
	{
		XY = 2,
		XYZ = 3,
		RGBA = 4,
		UV = 2
	};

	enum class DataType
	{
		IntData = GL_INT,
		FloatData = GL_FLOAT,
		UintData = GL_UNSIGNED_INT
	};

	enum class VBO
	{
		VertexBuffer,
		ColorBuffer,
		TextureBuffer,
		NormalBuffer
	};

	enum class RenderMode
	{
		Lines = GL_LINES,
		Triangles = GL_TRIANGLES,
		LineLoop = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN
	};

	static void SetLineWidth(GLfloat lineWidth);

	Buffer(GLsizei totalVertices, bool hasEBO = false);
	~Buffer();

	void FillEBO(const GLuint* data,
		GLsizeiptr bufferSize, Fill fill = Fill::Once);
	void FillVBO(VBO vbo, const void* data,
		GLsizeiptr bufferSize, Fill fill = Fill::Once);

	void AppendEBO(const GLuint* data,
		GLsizeiptr size, GLuint offset);
	void AppendVBO(VBO vbo,
		const void* data, GLsizeiptr size, GLuint offset);

	void LinkEBO();
	void LinkVBO(GLint attributeID,
		VBO vbo, ComponentSize componentSize, DataType dataType);

	void Render(RenderMode renderMode,
		GLuint index = 0, GLuint totalVertices = 0);

private:

	GLuint VAO{ 0 };
	GLuint EBO{ 0 };
	GLuint VBOs[4]{ 0, 0, 0, 0 };
	
	bool hasEBO{ false };
	GLsizei totalVertices{ 0 };

};