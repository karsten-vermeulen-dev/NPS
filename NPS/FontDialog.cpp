#include "FontDialog.h"
#include "Utility.h"

//======================================================================================================
FontDialog::FontDialog(const std::string& filename)
{
	if (!Utility::LoadConfigFile(filename, fontNames))
	{
		//assert if failed to load?
	}

	//If we remove the SAMPLE text, this will become redundant
	//We need to load the fonts into the UI system so that we 
	//can use them when writing 'Sample' in the font dialog box
	for (auto& font : fontNames)
	{
		if (!UserInterface::LoadFont(font.second, 24.0f, font.first))
		{
			//assert if failed to load?
		}
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
	std::vector<const char*> fontsTemp;

	for (auto& fontName : fontNames)
	{
		fontsTemp.push_back(fontName.first.c_str());
	}

	//For all buttons we indent using a formula and we must subtract 
	//the window padding because that will have been included by ImGUI
	const auto buttonIndent = (dimension.x * 0.5f) - 
		                      (buttonDimension.x * 0.5f) - 
		                      ImGui::GetStyle().WindowPadding.x;

	const auto sampleTextIndent = 117;
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

	//We clear the font so that everytime we click on 
	//a radio button the font won't automatically apply
	if (ImGui::RadioButton("Registration", fontToChange.isRegistration))
	{
		font.clear();
		fontToChange.isRegistration = true;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = false;
		fontToChange.isDealerPostcodeBSAU = false;
	}

	if (ImGui::RadioButton("Dealer", fontToChange.isDealer))
	{
		font.clear();
		fontToChange.isRegistration = false;
		fontToChange.isDealer = true;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = false;
		fontToChange.isDealerPostcodeBSAU = false;
	}

	if (ImGui::RadioButton("Postcode", fontToChange.isPostcode))
	{
		font.clear();
		fontToChange.isRegistration = false;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = true;
		fontToChange.isBSAU = false;
		fontToChange.isDealerPostcodeBSAU = false;
	}

	if (ImGui::RadioButton("BSAU", fontToChange.isBSAU))
	{
		font.clear();
		fontToChange.isRegistration = false;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = true;
		fontToChange.isDealerPostcodeBSAU = false;
	}

	if (ImGui::RadioButton("Dealer/Postcode/BSAU", fontToChange.isDealerPostcodeBSAU))
	{
		font.clear();
		fontToChange.isRegistration = false;
		fontToChange.isDealer = false;
		fontToChange.isPostcode = false;
		fontToChange.isBSAU = false;
		fontToChange.isDealerPostcodeBSAU = true;
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
		ImGui::Combo(" ", &fontID, &fontsTemp[0], totalFonts);

		Partition("", "", yellow);

		ImGui::PushFont(fonts[fontsTemp[fontID]]);

		ImGui::Spacing();

		//This could be a function
		ImGui::Indent(sampleTextIndent);
		ImGui::Text("Sample");
		ImGui::Unindent(sampleTextIndent);

		ImGui::PopFont();
	}

	//===================================================================================

	Spacing(spacingBetweenSections);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Spacing();
	ImGui::Separator();

	Spacing(spacingToButton);
	
	ImGui::Indent(buttonIndent);

	//This is the desired font, hide the dialog box
	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
		font = fontNames[fontsTemp[fontID]];
	}

	//Go back to the main screen, hide the dialog box
	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		font.clear();
		isVisible = false;
	}
	
	//This is the desired font, continue to display the dialog box
	if (ImGui::Button("Apply", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		font = fontNames[fontsTemp[fontID]];
	}

	//===================================================================================
	
	//slider for text sizes?
	//static int size{ 0 };
	//ImGui::SliderInt("Text size", &size, 2, );

	//===================================================================================

	ImGui::End();

}