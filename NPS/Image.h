#pragma once

#include "GLAD/gl.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

class Image
{

public:

	Image(const std::string& filename);
	virtual ~Image() {}

	Transform& GetTransform();

	void SetDimension(GLfloat width, GLfloat height);

	void Update();
	virtual void Render(Shader& shader, bool isLit = false);

private:

	Buffer buffer;
	Texture texture;
	Transform transform;

	GLfloat shininess{ 64.0f };

	glm::vec3 ambient{ 0.25f };
	glm::vec3 diffuse{ 0.3f };
	glm::vec3 specular{ 0.5f, 0.5f, 0.1f };
	glm::mat3 normalMatrix{ 1.0f };

};