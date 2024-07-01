#include "PropertiesPanel.h"
#include "Screen.h"

//======================================================================================================
PropertiesPanel::Properties& PropertiesPanel::GetProperties()
{
	return properties;
}
//======================================================================================================
void PropertiesPanel::Show()
{
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("PropertiesPanel", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("PropertiesPanel", ImVec2(position.x, position.y));
	ImGui::SetWindowSize("PropertiesPanel", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	auto DisplayFontDialog = []()
	{
		//HFONT hfont, hfontPrev;
		//DWORD previousTextColor;
		//HDC deviceContext;             // display device context of owner window

		CHOOSEFONT fontDialog;           // common dialog box structure
		static LOGFONT logFont;          // logical font structure
		static DWORD currentTextColor;   // current text color

		ZeroMemory(&fontDialog, sizeof(fontDialog));
		fontDialog.lStructSize = sizeof(fontDialog);
		fontDialog.hwndOwner = Screen::Instance()->GetWindowHandle();
		fontDialog.lpLogFont = &logFont;
		fontDialog.rgbColors = currentTextColor;
		fontDialog.Flags = CF_SCREENFONTS | CF_EFFECTS;

		if (ChooseFont(&fontDialog) == TRUE)
		{
			/*hfont = CreateFontIndirect(cf.lpLogFont);
			hfontPrev = SelectObject(hdc, hfont);
			rgbCurrent = cf.rgbColors;
			rgbPrev = SetTextColor(hdc, rgbCurrent);*/
		}
	};

	//===================================================================================

	const auto spacing = 2;

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Registration", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	char regText[20];
	strcpy_s(regText, properties.registrationText.c_str());
	ImGui::InputText("Enter Reg", regText, 20, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsUppercase);
	properties.registrationText = regText;

	ImGui::SliderInt("Raise text", &properties.raisedRegistration, -10, 10);
	ImGui::SliderInt("Nudge text", &properties.nudgedRegistration, -10, 10);

	ImGui::BeginDisabled(!properties.isTwoLineRegistration);
	ImGui::SliderInt("Raise space", &properties.raisedTwoLineSpace, -13, 13);
	ImGui::EndDisabled();

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Two-line registration", &properties.isTwoLineRegistration);

	ImGui::PopFont();

	//===================================================================================

	ImGui::BeginDisabled(!isActive);

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Font", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Car font", &properties.isCarFont);

	if (ImGui::RadioButton("2D Regular", properties.is2DRegular))
	{
		properties.is2DRegular = true;
		properties.is3DGelResin = false;
		properties.is4DLaserCut = false;
	}

	if (ImGui::RadioButton("3D Gel/Resin", properties.is3DGelResin))
	{
		properties.is2DRegular = false;
		properties.is3DGelResin = true;
		properties.is4DLaserCut = false;
	}

	if (ImGui::RadioButton("4D Laser cut", properties.is4DLaserCut))
	{
		properties.is2DRegular = false;
		properties.is3DGelResin = false;
		properties.is4DLaserCut = true;
	}

	if (ImGui::Button("Custom font", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		DisplayFontDialog();
	}

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Plate", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::SliderInt("Width", &properties.plateWidth, 2, 650);
	ImGui::SliderInt("Height", &properties.plateHeight, 2, 250);

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Border", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display border", &properties.isBorderVisible);
	
	ImGui::BeginDisabled(!properties.isBorderVisible);
	ImGui::Checkbox("Add side badge", &properties.isSideBadgeVisible);
	ImGui::SliderInt("Border size", &properties.borderSize, 1, 6);
	ImGui::SliderInt("Margin size", &properties.marginSize, 0, 9);
	ImGui::EndDisabled();

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Dealer", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display dealer", &properties.isDealerVisible);

	ImGui::BeginDisabled(!properties.isDealerVisible);
	ImGui::Checkbox("Dealer above postcode", &properties.isDealerAbovePostcode);
	ImGui::SliderInt("Raise dealer", &properties.raisedDealer, -10, 10);
	ImGui::SliderInt("Nudge dealer", &properties.nudgedDealer, -10, 10);
	ImGui::SliderInt("Raise postcode", &properties.raisedPostcode, -10, 10);
	ImGui::SliderInt("Nudge postcode", &properties.nudgedPostcode, -10, 10);

	char dealerText[20];
	strcpy_s(dealerText, properties.dealerText.c_str());
	ImGui::InputText("Enter dealer", dealerText, 20);
	properties.dealerText = dealerText;

	char postcodeText[20];
	strcpy_s(postcodeText, properties.postcodeText.c_str());
	ImGui::InputText("Enter postcode", postcodeText, 20, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsUppercase);
	properties.postcodeText = postcodeText;

	if (ImGui::Button("Dealer font", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		DisplayFontDialog();
	}

	ImGui::SameLine();

	if (ImGui::Button("Postcode font", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		DisplayFontDialog();
	}

	ImGui::EndDisabled();
	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "BSAU", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display BSAU", &properties.isBSAUVisible);

	ImGui::BeginDisabled(!properties.isBSAUVisible);
	ImGui::Checkbox("BSAU on border", &properties.isBSAUOnBorder);
	ImGui::SliderInt("Raise BSAU", &properties.raisedBSAU, -10, 10);
	ImGui::SliderInt("Nudge BSAU", &properties.nudgedBSAU, -10, 10);

	if (ImGui::Button("BSAU font", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		DisplayFontDialog();
	}

	ImGui::EndDisabled();
	ImGui::PopFont();

	//===================================================================================

	ImGui::EndDisabled();
	ImGui::End();
}