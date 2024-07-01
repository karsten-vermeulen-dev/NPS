#include <gtc\matrix_transform.hpp>
#include "Transform.h"

//======================================================================================================
glm::vec3 Transform::GetEulerAngles()
{
	return glm::degrees(glm::eulerAngles(rotation));
}
//======================================================================================================
const glm::vec3& Transform::GetPosition() const
{
	return position;
}
//======================================================================================================
GLfloat Transform::GetScale() const
{
	return scale;
}
//======================================================================================================
const glm::mat4& Transform::GetMatrix()
{
	Update();
	return matrix;
}
//======================================================================================================
void Transform::SetIdentity()
{
	scale = 1.0f;
	isDirty = false;
	matrix = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
//======================================================================================================
void Transform::SetPosition(const glm::vec3& position)
{
	this->position = position;
	isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(const glm::vec3& eulerAngles)
{
	rotation = glm::quat(glm::radians(eulerAngles));
	isDirty = true;
}
//======================================================================================================
void Transform::SetScale(GLfloat scale)
{
	this->scale = scale;
	isDirty = true;
}
//======================================================================================================
void Transform::Update()
{
	if (isDirty)
	{
		matrix = glm::translate(glm::mat4(1.0f), position);
		matrix *= glm::mat4_cast(rotation);
		matrix = glm::scale(matrix, glm::vec3(scale));
		isDirty = false;
	}
}