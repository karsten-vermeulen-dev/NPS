#include "PrintPanel.h"

//======================================================================================================
const PrintPanel::Metrics& PrintPanel::GetMetrics() const
{
	return metrics;
}
//======================================================================================================
const PrintPanel::ButtonState& PrintPanel::GetButtonState()
{
	return buttonState;
}
//======================================================================================================
void PrintPanel::Show()
{
	ImGui::Begin("PrintPanel", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("PrintPanel", ImVec2(position.x, position.y));
	ImGui::SetWindowSize("PrintPanel", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	ImGui::BeginDisabled(!isActive);

	Partition("MyriadPro_Bold_16", "Margin", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::SliderInt("Top", &metrics.topMargin, -25, 25);
	ImGui::SliderInt("Left", &metrics.leftMargin, -25, 25);

	//===================================================================================

	Spacing(3);
	Partition("MyriadPro_Bold_16", "Character", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::SliderInt("Wide width", &metrics.wideCharacterWidth, -10, 10);
	ImGui::SliderInt("Wide height", &metrics.wideCharacterHeight, -10, 10);
	ImGui::SliderInt("Narrow width", &metrics.narrowCharacterWidth, -10, 10);

	//===================================================================================

	Spacing(3);

	buttonState.print = false;

	if (ImGui::Button("Print", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.print = true;
	}

	//===================================================================================

	ImGui::EndDisabled();

	ImGui::End();
}