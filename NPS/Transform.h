#pragma once

#include "GLAD/gl.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

class Transform
{

public:

	GLfloat GetScale() const;
	glm::vec3 GetEulerAngles();
	const glm::vec3& GetPosition() const;
	const glm::mat4& GetMatrix();

	void SetIdentity();
	void SetScale(GLfloat scale);
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& eulerAngles);

	void Update();

private:

	bool isDirty{ false };
	
	GLfloat scale{ 1.0f };
	glm::mat4 matrix{ 1.0f };
	glm::vec3 position{ 0.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

};