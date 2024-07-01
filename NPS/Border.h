#pragma once

#include <memory>
#include "GLAD/gl.h"
#include "Buffer.h"
#include "Shader.h"

class Plate;

class Border
{

public:

	Border(Plate* parent);
	~Border() {}

	void Render(Shader& shader);

private:

	void FillBuffers();

	bool isDirty{ true };
	Plate* parent{ nullptr };
	GLfloat lineThickness{ 0.0f };

	//We have to manually create this object 
	//AFTER calculating the total vertices
	std::unique_ptr<Buffer> buffer;

};