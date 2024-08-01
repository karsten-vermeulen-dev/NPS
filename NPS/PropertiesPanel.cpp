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

	ImGui::BeginDisabled(!isActive);

	const auto spacing = 2;

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

	ImGui::EndDisabled();
	ImGui::PopFont();

	//===================================================================================

	ImGui::EndDisabled();
	ImGui::End();
}
//======================================================================================================
void PropertiesPanel::Reset()
{
	properties.plateWidth = 520;
	properties.plateHeight = 111;

	properties.isTwoLineRegistration = false;

	properties.raisedRegistration = 0;
	properties.nudgedRegistration = 0;
	properties.raisedTwoLineSpace = 0;

	properties.registrationText = "REG 123";

	properties.borderSize = 2;
	properties.marginSize = 2;

	properties.isBorderVisible = true;
	properties.isSideBadgeVisible = false;

	properties.raisedDealer = 0;
	properties.nudgedDealer = 0;
	properties.raisedPostcode = 0;
	properties.nudgedPostcode = 0;

	properties.isDealerVisible = true;
	properties.isDealerAbovePostcode = false;

	properties.postcodeText = "B6 7SJ";
	properties.dealerText = "Plate factory";

	properties.raisedBSAU = 0;
	properties.nudgedBSAU = 0;

	properties.isBSAUVisible = true;
	properties.isBSAUOnBorder = true;

	properties.BSAUText = "NPM BS AU 145e";

}