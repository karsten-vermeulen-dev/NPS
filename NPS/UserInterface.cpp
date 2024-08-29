#include "UserInterface.h"

std::map<std::string, ImFont*> UserInterface::fonts;

ImVec4 UserInterface::red = ImVec4(1.0f, 0.0f, 0.098f, 1.0f);
ImVec4 UserInterface::white = ImVec4(0.88f, 0.88f, 0.88f, 1.0f);
ImVec4 UserInterface::yellow = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);
ImVec4 UserInterface::darkGrey = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
ImVec4 UserInterface::lightGrey = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);

//======================================================================================================
ImVec4 UserInterface::CustomColor(int r, int g, int b, int a)
{
	return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
//======================================================================================================
bool UserInterface::LoadFont(const std::string& fontFilename, float fontSize, const std::string& tag)
{
	auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFilename.c_str(), fontSize);

	if (!font)
	{
		//Flag should be used by calling program and dealt with accordingly
		return false;
	}

	fonts[tag] = font;

	return true;
}
//======================================================================================================
void UserInterface::Spacing(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		ImGui::Spacing();
	}
}
//======================================================================================================
void UserInterface::Partition(const std::string& fontTag, const std::string& title, const ImVec4& color)
{
	ImGui::GetStyle().Colors[ImGuiCol_Text] = color;

	ImGui::PushFont(fonts[fontTag]);
	ImGui::Text(title.c_str());
	ImGui::PopFont();

	ImGui::Separator();
	Spacing(1);
}
//======================================================================================================
void UserInterface::Update()
{
	//For all buttons we indent using a formula and we must subtract 
	//the window padding because that will have been included by ImGUI
	buttonIndent = (dimension.x * 0.5f) -
				   (buttonDimension.x * 0.5f) -
				   ImGui::GetStyle().WindowPadding.x;
}
//======================================================================================================
bool UserInterface::IsVisible() const
{
	return isVisible;
}
//======================================================================================================
void UserInterface::IsVisible(bool flag)
{
	isVisible = flag;
}
//======================================================================================================
void UserInterface::IsActive(bool flag)
{
	isActive = flag;
}
//======================================================================================================
const std::string& UserInterface::GetTag() const
{
	return tag;
}
//======================================================================================================
void UserInterface::SetTag(const std::string& tag)
{
	this->tag = tag;
}
//======================================================================================================
void UserInterface::SetPosition(const glm::uvec2& position)
{
	this->position = position;
}
//======================================================================================================
void UserInterface::SetDimension(const glm::uvec2& dimension)
{
	this->dimension = dimension;
	Update();
}
//======================================================================================================
void UserInterface::SetButtonDimension(const glm::uvec2& dimension)
{
	buttonDimension = dimension;
	Update();
}