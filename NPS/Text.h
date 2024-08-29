#pragma once

#include <map>
#include <string>

#include <ft2build.h>
#include <freetype\freetype.h>
#include "GLAD/gl.h"
#include <glm.hpp>

#include "Buffer.h"
#include "Shader.h"
#include "Transform.h"

class Plate;

class Text
{

public:

	static bool Initialize();
	static void Shutdown();
	
	static bool Load(const std::string& filename,
					 GLfloat fontHeight,
					 const glm::vec2& maxDimension,
					 const std::string& tag);

	static void Unload(const std::string& tag);

	Text(Plate* parent, const std::string& tag);

	Transform& GetTransform();
	GLfloat GetMaxWidth() const;
	GLfloat GetMaxHeight() const;
	const std::string& GetString() const;

	void SetFont(const std::string& tag);
	void SetColor(const glm::vec4& color);
	void SetString(const std::string& string);

	void Render(Shader& shader);

private:

	struct Glyph
	{
		GLuint ID;					//OGL texture ID for each letter 
		GLint width;				//Width of the letter in px
		GLint height;				//Height of the letter in px
		GLint bearingX;				//Distance from the y-axis origin in px
		GLint bearingY;				//Distance from the x-axis baseline in px
		GLint advance;				//Offset to advance to next glyph in px
		GLfloat heightMillimeter;   //Height of the letter in mm
	};

	//Would love to call it 'Font' but that name is reserved
	typedef std::map<GLchar, Glyph> FontType;

	static FT_Library freetypeObject;
	static std::map<std::string, FontType> fonts;

	std::string string;

	GLfloat height{ 0.0f };
	glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };

	FontType font;
	Buffer buffer;
	Transform transform;
	Plate* parent{ nullptr };
};