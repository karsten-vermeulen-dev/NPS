#include "ModePanel.h"

//======================================================================================================
const ModePanel::Mode& ModePanel::GetMode()
{
	return mode;
}
//======================================================================================================
void ModePanel::Show()
{
	ImGui::Begin("ModePanel", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("ModePanel", ImVec2(position.x, position.y));
	ImGui::SetWindowSize("ModePanel", ImVec2(dimension.x, dimension.y));

	ImGui::GetStyle().Colors[ImGuiCol_Text] = yellow;

	Partition("MyriadPro_Bold_16", "ModePanel", yellow);

	ImGui::PushFont(fonts["MyriadPro_Bold_16"]);

	Spacing(2);
	ImGui::Indent(7);

	if (ImGui::RadioButton("Design", mode.isDesign))
	{
		mode.isPrint = false;
		mode.isDesign = true;
		mode.is3DView = false;
	}

	Spacing(3);

	if (ImGui::RadioButton("Print", mode.isPrint))
	{
		mode.isPrint = true;
		mode.isDesign = false;
		mode.is3DView = false;
	}

	Spacing(3);

	if (ImGui::RadioButton("3D view", mode.is3DView))
	{
		mode.isPrint = false;
		mode.isDesign = false;
		mode.is3DView = true;
	}

	ImGui::PopFont();
	ImGui::End();
}