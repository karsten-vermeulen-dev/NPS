#include "CustomDialog.h"

//======================================================================================================
const CustomDialog::CustomStyle& CustomDialog::GetCustomStyle()
{
	return customStyle;
}
//======================================================================================================
void CustomDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;

	ImGui::Begin("Custom plate",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("Custom plate", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	buttonState = { false };
	
	const auto buttonIndent = 100;
	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Custom number plate", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Select options below to create your own plate.");

	Spacing(spacingFromTitle);

	Partition("MyriadPro_Bold_16", "Vehicle", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	if (ImGui::RadioButton("Car", customStyle.isCar))
	{
		customStyle.isCar = true;
	}

	if (ImGui::RadioButton("Motorcycle", !customStyle.isCar))
	{
		customStyle.isCar = false;
	}

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacingBetweenSections);

	Partition("MyriadPro_Bold_16", "Font", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	if (ImGui::RadioButton("Car font", customStyle.hasCarFont))
	{
		customStyle.hasCarFont = true;
	}

	if (ImGui::RadioButton("Motorcycle font", !customStyle.hasCarFont))
	{
		customStyle.hasCarFont = false;
	}

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacingBetweenSections);

	Partition("MyriadPro_Bold_16", "Dimension", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::SliderInt("Width (mm)", &customStyle.dimension.x, 1, 616);
	ImGui::SliderInt("Height (mm)", &customStyle.dimension.y, 1, 203);

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacingBetweenSections);

	Partition("MyriadPro_Bold_16", "Other", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::Checkbox("Side badge", &customStyle.hasSideBadge);
	ImGui::Checkbox("Two line plate", &customStyle.isTwoLinePlate);

	ImGui::Spacing();
	ImGui::Separator();
	Spacing(spacingToButton);

	ImGui::Indent(buttonIndent);

	if (ImGui::Button("OK", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.ok = true;
		isVisible = false;
	}

	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.cancel = true;
		isVisible = false;
	}

	ImGui::PopFont();

	//===================================================================================

	ImGui::End();
}