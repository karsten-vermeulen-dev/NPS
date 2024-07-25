#include "FontDialog.h"
#include "Utility.h"

//======================================================================================================
FontDialog::FontDialog(const std::string& filename)
{
	if (!Utility::LoadConfigFile(filename, fontNames))
	{
		//assert if failed to load?
	}
}
//======================================================================================================
const std::string& FontDialog::GetFont() const
{
	return font;
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
	const auto totalFonts = fontNames.size();
	std::vector<const char*> fontList;

	for (auto& fontName : fontNames)
	{
		fontList.push_back(fontName.first.c_str());
	}

	font = fontNames[fontList[fontID]];

	//For all buttons we indent using a formula and we must subtract 
	//the window padding because that will have been included by ImGUI
	const auto buttonIndent = (dimension.x * 0.5f) - 
		                      (buttonDimension.x * 0.5f) - 
		                      ImGui::GetStyle().WindowPadding.x;

	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;

	//===================================================================================
	
	Partition("MyriadPro_Bold_16", "Font settings", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Select options below to adjust the registration font.");

	//===================================================================================
	
	Spacing(spacingFromTitle);

	Partition("MyriadPro_Bold_16", "Font to change", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	if (ImGui::RadioButton("Registration", fontToChange.isRegistration))
	{
		fontToChange.isRegistration = true;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = false;
	}

	if (ImGui::RadioButton("Dealer", fontToChange.isDealer))
	{
		fontToChange.isRegistration = false;
		fontToChange.isDealer = true;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = false;
	}

	if (ImGui::RadioButton("Postcode", fontToChange.isPostcode))
	{
		fontToChange.isRegistration = false;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = true;
		fontToChange.isBSAU = false;
	}

	if (ImGui::RadioButton("BSAU", fontToChange.isBSAU))
	{
		fontToChange.isRegistration = false;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = true;
	}

	ImGui::PopFont();

	//===================================================================================
	
	if (fontToChange.isRegistration)
	{
		Spacing(spacingFromTitle);

		Partition("MyriadPro_Bold_16", "Font type", yellow);

		ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
		ImGui::PushFont(fonts["MyriadPro_Regular"]);

		if (ImGui::RadioButton("Car", fontType.isCar))
		{
			fontType.isCar = true;
			fontType.isMotorCycle = false;
			fontType.isCustom = false;
		}

		if (ImGui::RadioButton("Motorcycle", fontType.isMotorCycle))
		{
			fontType.isCar = false;
			fontType.isMotorCycle = true;
			fontType.isCustom = false;
		}

		if (ImGui::RadioButton("Custom", fontType.isCustom))
		{
			fontType.isCar = false;
			fontType.isMotorCycle = false;
			fontType.isCustom = true;
		}

		ImGui::PopFont();
	}

	//===================================================================================
	
	//We only display the 2D/3D/4D radio button selection 
	//if the 'car' or 'motorcycle' font has been selected
	if (fontToChange.isRegistration && (fontType.isCar || fontType.isMotorCycle))
	{
		Spacing(spacingFromTitle);

		Partition("MyriadPro_Bold_16", "Font style", yellow);

		ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
		ImGui::PushFont(fonts["MyriadPro_Regular"]);

		if (ImGui::RadioButton("2D Regular", fontStyle.is2DRegular))
		{
			fontStyle.is2DRegular = true;
			fontStyle.is3DGelResin = false;
			fontStyle.is4DLaserCut = false;
		}

		if (ImGui::RadioButton("3D Gel/Resin", fontStyle.is3DGelResin))
		{
			fontStyle.is2DRegular = false;
			fontStyle.is3DGelResin = true;
			fontStyle.is4DLaserCut = false;
		}

		if (ImGui::RadioButton("4D Laser cut", fontStyle.is4DLaserCut))
		{
			fontStyle.is2DRegular = false;
			fontStyle.is3DGelResin = false;
			fontStyle.is4DLaserCut = true;
		}

		ImGui::PopFont();
	}
	
	//We only display the font selection combo box if we want to change the 
	//dealer/postcode/BSAU sections or if the registration requires a custom font
	if (!fontToChange.isRegistration || (fontToChange.isRegistration && fontType.isCustom))
	{
		Spacing(spacingFromTitle);
		Partition("MyriadPro_Bold_16", "Font name", yellow);

		ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
		ImGui::Combo(" ", &fontID, &fontList[0], totalFonts);
	}

	//===================================================================================

	Spacing(spacingBetweenSections);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Spacing();
	ImGui::Separator();

	Spacing(spacingToButton);
	
	ImGui::Indent(buttonIndent);

	//Go back to the main screen, hide the dialog box, keep the new settings
	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
		buttonState.ok = true;
		buttonState.cancel = false;
	}

	//Go back to the main screen, hide the dialog box, revert to original settings
	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		font.clear();
		isVisible = false;
		buttonState.ok = false;
		buttonState.cancel = true;
	}
	
	//===================================================================================
	
	//slider for text sizes?
	//static int size{ 0 };
	//ImGui::SliderInt("Text size", &size, 2, );

	//===================================================================================

	ImGui::End();

}