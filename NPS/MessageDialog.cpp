#include "MessageDialog.h"

//======================================================================================================
const MessageDialog::ButtonState& MessageDialog::GetButtonState()
{
	return buttonState;
}
//======================================================================================================
void MessageDialog::SetTitle(const std::string& title)
{
	this->title = title;
}
//======================================================================================================
void MessageDialog::SetMessage(const std::string& message)
{
	this->message = message;
}
//======================================================================================================
void MessageDialog::SetButtonType(ButtonType buttonType)
{
	this->buttonType = buttonType;
}
//======================================================================================================
void MessageDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;

	ImGui::Begin(title.c_str(),
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize(title.c_str(), ImVec2(dimension.x, dimension.y));

	//===================================================================================

	const auto spacingToButton = 5;
	
	//===================================================================================
	
	Partition("MyriadPro_Bold_16", title, yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::Text(message.c_str());

	Spacing(spacingToButton);

	ImGui::Indent(buttonIndent);

	buttonState = { false };

	if ((buttonType == ButtonType::Okay))
	{
		if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
		{
			buttonState.ok = true;
		}
	}

	else if (buttonType == ButtonType::YesNo)
	{
		if (ImGui::Button("Yes", ImVec2(buttonDimension.x, buttonDimension.y)))
		{
			buttonState.yes = true;
		}

		if (ImGui::Button("No", ImVec2(buttonDimension.x, buttonDimension.y)))
		{
			buttonState.no = true;
		}
	}

	else if (buttonType == ButtonType::OkCancel)
	{
		if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
		{
			buttonState.ok = true;
		}

		if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
		{
			buttonState.cancel = true;
		}
	}

	ImGui::PopFont();

	//===================================================================================

	ImGui::End();
}