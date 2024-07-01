#include "Design.h"
#include "Input.h"
#include "Tutorial.h"

//======================================================================================================
bool Tutorial::OnEnter()
{
	mainShader = std::make_unique<Shader>();

	if (!mainShader->Create("Shaders/Main.vert", "Shaders/Main.frag"))
	{
		return false;
	}

	mainShader->BindAttribute("vertexIn");
	mainShader->BindAttribute("colorIn");
	mainShader->BindAttribute("textureIn");

	mainShader->BindUniform("model");
	mainShader->BindUniform("isText");
	mainShader->BindUniform("isTextured");
	mainShader->BindUniform("textureImage");

	image = std::make_unique<Image>("Assets/Images/Tutorial/Tutorial.png");

	return true;
}
//======================================================================================================
State* Tutorial::Update()
{
	if (Input::Instance()->IsXClicked())
	{
		return nullptr;
	}

	return this;
}
//======================================================================================================
bool Tutorial::Render()
{
	auto resolution = static_cast<glm::vec2>(Screen::Instance()->GetMainResolution());

	mainShader->Use();

	Screen::Instance()->SetViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);
	Screen::Instance()->Refresh();

	image->Render(*mainShader);

	return true;
}
//======================================================================================================
void Tutorial::OnExit()
{
	//Do we need to destroy this everytime we switch states?
	mainShader->Destroy();
}