#pragma once

#include <memory>
#include "Image.h"
#include "Shader.h"
#include "State.h"

class Tutorial : public State
{

public:

	virtual ~Tutorial() {}
	virtual bool OnEnter();
	virtual State* Update();
	virtual bool Render();
	virtual void OnExit();

private:

	std::unique_ptr<Image> image;
	std::unique_ptr<Shader> mainShader;

};