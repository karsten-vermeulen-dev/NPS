#pragma once

#include <string>
#include "GLAD/gl.h"

class Texture
{

public:

	Texture(const std::string& filename);
	~Texture();

	void Bind() const;
	void Unbind() const;

private:

	GLuint ID{ 0 };

};