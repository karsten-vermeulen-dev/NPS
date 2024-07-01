#include "AboutDialog.h"

//======================================================================================================
void AboutDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;

	ImGui::Begin("AboutDialog",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("AboutDialog", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	const auto buttonIndent = 90;
	const auto spacingToButton = 5;
	
	//===================================================================================

	Partition("MyriadPro_Bold_16", "About application", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::Text("Number plate software. Copyright 2024.");

	Spacing(spacingToButton);

	ImGui::Indent(buttonIndent);

	if (ImGui::Button("OK", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}

	ImGui::PopFont();

	//===================================================================================

	ImGui::End();
}