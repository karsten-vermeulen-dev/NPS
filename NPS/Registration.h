#pragma once

#include <map>
#include <string>
#include <glm.hpp>
#include "GLAD/gl.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

class Plate;

class Registration
{

public:

	//For both car and motorcycle there is a 11mm gap between the
	//letters, a 33mm spacing between groups when the space bar is
	//hit and a 13mm gap between lines of text on two-line plates 
	const GLuint twoLineSpacing = 13;
	const GLuint spacebarSpacing = 33;
	const GLuint characterSpacing = 11;

	static void Load(const std::string& filename, 
		            const glm::vec2& maxDimension, 
		            const std::string& tag);

	static void Unload(const std::string& tag);

	Registration(Plate* parent);

	//This isn't const because the caller needs 
	//to make a change to the transformation
	Transform& GetTransform();

	const std::string& GetString() const;

	void SetFont(const std::string& tag);
	void SetString(const std::string& string);

	void Render(Shader& shader);

private:

	struct Glyph
	{
		Glyph(const std::string& filename = "")
			: width(0), height(0), texture(filename) {}

		GLfloat width{ 0.1f };
		GLfloat height{ 0.1f };
		Texture texture;
	};

	//Would love to call it 'Font' but that name is reserved
	typedef std::map<GLchar, Glyph> FontType;

	static GLuint totalRegObjects;
	static std::map<std::string, FontType> fonts;

	void Render(Shader& shader, const std::string& registration, glm::vec3& position);

	std::string string;

	Buffer buffer;
	FontType font;
	Transform transform;
	Plate* parent{ nullptr };

};