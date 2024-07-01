#include "LicenseDialog.h"

const LicenseDialog::ButtonState& LicenseDialog::GetButtonState()
{
	return buttonState;
}

void LicenseDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;

	ImGui::Begin("Check your license",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("Check your license", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	buttonState = { false };

	const auto buttonIndent = 100;
	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Check your license", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Information about your current license.");

	Spacing(spacingFromTitle);

	Partition("MyriadPro_Bold_16", "Company", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	ImGui::Text("Plate Factory");

	Spacing(spacingBetweenSections);

	ImGui::PopFont();

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Email", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	ImGui::Text("platefactory@nps.com");
	
	Spacing(spacingBetweenSections);

	ImGui::PopFont();

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Licence key", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	ImGui::Text("0123456789");
	ImGui::Text("Valid until 31 December 2024");

	ImGui::Spacing();
	ImGui::Separator();

	Spacing(spacingToButton);
	ImGui::Indent(buttonIndent);

	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.okay = true;
		isVisible = false;
	}

	ImGui::PopFont();

	//===================================================================================

	ImGui::End();
}