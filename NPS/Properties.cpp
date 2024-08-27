#include "Properties.h"
#include "Screen.h"

//======================================================================================================
//Properties::Properties& Properties::GetProperties()
//{
//	return properties;
//}
//======================================================================================================
void Properties::Show()
{
	ImGui::GetStyle().WindowRounding = 0.0f;

	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos("Properties", ImVec2(position.x, position.y));
	ImGui::SetWindowSize("Properties", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	ImGui::BeginDisabled(!isActive);

	const auto spacing = 2;

	Partition("MyriadPro_Bold_16", "Registration", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	char regText[20];
	strcpy_s(regText, registrationText.c_str());
	ImGui::InputText("Enter Reg", regText, 20, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsUppercase);
	registrationText = regText;

	ImGui::SliderInt("Raise text", &raisedRegistration, -10, 10);
	ImGui::SliderInt("Nudge text", &nudgedRegistration, -10, 10);

	ImGui::BeginDisabled(!isTwoLineRegistration);
	ImGui::SliderInt("Raise space", &raisedTwoLineSpace, -13, 13);
	ImGui::EndDisabled();

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Two-line registration", &isTwoLineRegistration);

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Plate", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);

	ImGui::Text(("Name: " + plateName).c_str());

	ImGui::SliderInt("Width", &plateWidth, 2, 650);
	ImGui::SliderInt("Height", &plateHeight, 2, 250);

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Border", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display border", &isBorderVisible);
	
	ImGui::BeginDisabled(!isBorderVisible);
	ImGui::Checkbox("Add side badge", &isSideBadgeVisible);
	ImGui::SliderInt("Border size", &borderSize, 1, 6);
	ImGui::SliderInt("Margin size", &marginSize, 0, 9);
	ImGui::EndDisabled();

	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Dealer", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display dealer", &isDealerVisible);

	ImGui::BeginDisabled(!isDealerVisible);
	ImGui::Checkbox("Dealer above postcode", &isDealerAbovePostcode);
	ImGui::SliderInt("Raise dealer", &raisedDealer, -10, 10);
	ImGui::SliderInt("Nudge dealer", &nudgedDealer, -10, 10);
	ImGui::SliderInt("Raise postcode", &raisedPostcode, -10, 10);
	ImGui::SliderInt("Nudge postcode", &nudgedPostcode, -10, 10);

	char dealerText[20];
	strcpy_s(dealerText, this->dealerText.c_str());
	ImGui::InputText("Enter dealer", dealerText, 20);
	this->dealerText = dealerText;

	char postcodeText[20];
	strcpy_s(postcodeText, this->postcodeText.c_str());
	ImGui::InputText("Enter postcode", postcodeText, 20, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsUppercase);
	this->postcodeText = postcodeText;

	ImGui::EndDisabled();
	ImGui::PopFont();

	//===================================================================================

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "BSAU", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
	ImGui::Checkbox("Display BSAU", &isBSAUVisible);

	ImGui::BeginDisabled(!isBSAUVisible);
	ImGui::Checkbox("BSAU on border", &isBSAUOnBorder);
	ImGui::SliderInt("Raise BSAU", &raisedBSAU, -10, 10);
	ImGui::SliderInt("Nudge BSAU", &nudgedBSAU, -10, 10);

	ImGui::EndDisabled();
	ImGui::PopFont();

	//===================================================================================
	
	const auto spacingToButton = 7;

	Spacing(spacing);
	Partition("MyriadPro_Bold_16", "Mode", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	
	Spacing(spacingToButton);

	ImGui::Indent(buttonIndent);

	if (ImGui::Button("Design", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.design = true;
		buttonState.printPreview = false;
		buttonState.view3D = false;
	}

	if (ImGui::Button("Print preview", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.design = false;
		buttonState.printPreview = true;
		buttonState.view3D = false;
	}

	if (ImGui::Button("3D view", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.design = false;
		buttonState.printPreview = false;
		buttonState.view3D = true;
	}

	//===================================================================================

	ImGui::EndDisabled();
	ImGui::End();

	//===================================================================================
	//===================================================================================

	//Migrated from older 'PrintPanel'
	/*ImGui::BeginDisabled(!isActive);

	Partition("MyriadPro_Bold_16", "Margin", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::SliderInt("Top", &metrics.topMargin, -25, 25);
	ImGui::SliderInt("Left", &metrics.leftMargin, -25, 25);*/

	//===================================================================================

	/*Spacing(3);
	Partition("MyriadPro_Bold_16", "Character", yellow);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

	ImGui::SliderInt("Wide width", &metrics.wideCharacterWidth, -10, 10);
	ImGui::SliderInt("Wide height", &metrics.wideCharacterHeight, -10, 10);
	ImGui::SliderInt("Narrow width", &metrics.narrowCharacterWidth, -10, 10);*/

	//===================================================================================

	/*Spacing(3);

	buttonState.print = false;

	if (ImGui::Button("Print", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		buttonState.print = true;
	}*/

	//===================================================================================

	//ImGui::EndDisabled();
}
//======================================================================================================
void Properties::Reset()
{
	plateWidth = 520;
	plateHeight = 111;

	isTwoLineRegistration = false;

	raisedRegistration = 0;
	nudgedRegistration = 0;
	raisedTwoLineSpace = 0;

	registrationText = "REG 123";

	borderSize = 2;
	marginSize = 2;

	isBorderVisible = true;
	isSideBadgeVisible = false;

	raisedDealer = 0;
	nudgedDealer = 0;
	raisedPostcode = 0;
	nudgedPostcode = 0;

	isDealerVisible = true;
	isDealerAbovePostcode = false;

	postcodeText = "B6 7SJ";
	dealerText = "Plate factory";

	raisedBSAU = 0;
	nudgedBSAU = 0;

	isBSAUVisible = true;
	isBSAUOnBorder = true;

	BSAUText = "NPM BS AU 145e";
}