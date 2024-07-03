#include "FontDialog.h"

//======================================================================================================
FontDialog::FontDialog()
{
	fontNames["Arial"] = "Assets/Fonts/Arial_bold.ttf";
	fontNames["MyriadPro"] = "Assets/Fonts/MyriadPro_bold.otf";
	fontNames["Calibri"] = "Assets/Fonts/Calibri_bold.ttf";
	fontNames["Comic"] = "Assets/Fonts/Comic_bold.ttf";
	fontNames["Gothic"] = "Assets/Fonts/Gothic_bold.ttf";
}
//======================================================================================================
const std::string& FontDialog::GetChosenFont() const
{
	return chosenFont;
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
	const auto totalFonts = 5;
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
	
	ImGui::PushFont(fonts[fontsTemp[fontID]]);

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

	//This is the desired font
	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
		chosenFont = fontNames[fontsTemp[fontID]];
	}

	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}
	
	//===================================================================================

	ImGui::End();

}