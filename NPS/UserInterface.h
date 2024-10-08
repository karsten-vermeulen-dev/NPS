#pragma once

#include <map>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm.hpp>

//Reference for style
//https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html

class UserInterface
{

public:

	static ImVec4 red;
	static ImVec4 white;
	static ImVec4 yellow;
	static ImVec4 darkGrey;
	static ImVec4 lightGrey;

	//Currently to help create new colors. Can eventually remove this
	static ImVec4 CustomColor(int r, int g, int b, int a = 255);

	//Is there an UnloadFont?
	static bool LoadFont(const std::string& fontFilename, float fontSize, const std::string& tag);

	virtual ~UserInterface() = 0 {}

	bool IsVisible() const;
	void IsVisible(bool flag);
	void IsActive(bool flag);

	const std::string& GetTag() const;

	void SetTag(const std::string& tag);
	void SetPosition(const glm::uvec2& position);
	void SetDimension(const glm::uvec2& dimension);
	void SetButtonDimension(const glm::uvec2& dimension);

	void Spacing(int amount);
	void Partition(const std::string& fontTag, const std::string& title = "", const ImVec4& color = white);

	virtual void Show() = 0;
	virtual void Reset() = 0;

private:

	//For the moment, this is for internal updates only, no need to call from anywhere
	//Not sure if this is neccessary to override later?
	void Update();

protected:

	static std::map<std::string, ImFont*> fonts;

	bool isActive{ true };
	bool isVisible{ false };

	std::string tag;

	float buttonIndent{ 0.0f };
	glm::vec2 position{ 0.0f };
	glm::vec2 dimension{ 0.0f };
	glm::vec2 buttonDimension{ 0.0f };

};