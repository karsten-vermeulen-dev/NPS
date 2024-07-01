#include "ActivateDialog.h"

//======================================================================================================
const ActivateDialog::ButtonState& ActivateDialog::GetButtonState()
{
	return buttonState;
}
//======================================================================================================
void ActivateDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;
	
	ImGui::Begin("Activate your program",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("Activate your program", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	buttonState = { false };

	const auto buttonIndent = 100;
	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;
	
	//===================================================================================
	
	Partition("MyriadPro_Bold_16", "Activate program", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Check information below and activate program.");

	Spacing(spacingFromTitle);
	
	Partition("MyriadPro_Bold_16", "Company", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	static char company[1000] = { "Plate Factory" };
	ImGui::InputText(" ", company, sizeof(company));

	Spacing(spacingBetweenSections);

	ImGui::PopFont();

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Email", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	static char email[1000] = { "platefactory@nps.com" };
	ImGui::InputText("  ", email, sizeof(email));

	Spacing(spacingBetweenSections);

	ImGui::PopFont();

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Licence key", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	
	static char licence[15] = { "0123456789" };
	ImGui::InputText("   ", licence, sizeof(licence));

	ImGui::Spacing();
	ImGui::Separator();
	
	Spacing(spacingToButton);
	ImGui::Indent(buttonIndent);

	if (ImGui::Button("Activate", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.activate = true;
		isVisible = false;
	}

	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.cancel = true;
		isVisible = false;
	}

	if (ImGui::Button("Help", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.help = true;
		isVisible = false;
	}

	ImGui::PopFont();

	//===================================================================================

	ImGui::End();
}