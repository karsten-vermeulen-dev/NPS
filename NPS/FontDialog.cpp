#include "FontDialog.h"

//======================================================================================================
FontDialog::FontDialog()
{
	fontNames.push_back(std::make_pair("Arial", "Arial_Bold_24"));
	fontNames.push_back(std::make_pair("MyriadPro", "MyriadPro_Bold_24"));
}
//======================================================================================================
void FontDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;

	ImGui::Begin("Font",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("Font", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	buttonState = { false };

	static int fontID = 0;
	const auto totalFonts = 2;
	const char* fontsTemp[totalFonts];

	auto count = 0;

	for (auto& fontName : fontNames)
	{
		fontsTemp[count++] = fontName.first.c_str();
	}

	const auto buttonIndent = 15;
	const auto sampleTextIndent = 90;
	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;

	//===================================================================================
	
	Partition("MyriadPro_Bold_16", "Font", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Choose your desired font.");

	Spacing(spacingFromTitle);
	Partition("MyriadPro_Bold_16", "Font name", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	//ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::Combo(" ", &fontID, fontsTemp, totalFonts);

	//ImGui::PopFont();

	//===================================================================================
	
	Spacing(spacingBetweenSections);

	Partition("", "", yellow);
	
	ImGui::PushFont(fonts[fontNames[fontID].second]);

	ImGui::Indent(sampleTextIndent);
	ImGui::Text("Sample text");

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacingBetweenSections);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;


	ImGui::Spacing();
	ImGui::Separator();

	Spacing(spacingToButton);
	
	ImGui::Indent(buttonIndent);

	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}

	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}
	
	//===================================================================================

	ImGui::End();

}