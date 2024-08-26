//#include "FeedbackPanel.h"
//
////======================================================================================================
//void FeedbackPanel::SetLegality(bool flag)
//{
//	isLegal = flag;
//}
////======================================================================================================
//void FeedbackPanel::SetPlateData(const Plate::PlateData& plateData)
//{
//	this->plateData = plateData;
//}
////======================================================================================================
//void FeedbackPanel::Show()
//{
//	ImGui::GetStyle().WindowRounding = 0.0f;
//
//	ImGui::Begin("FeedbackPanel", nullptr,
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse |
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
//		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
//
//	ImGui::SetWindowPos("FeedbackPanel", ImVec2(position.x, position.y));
//	ImGui::SetWindowSize("FeedbackPanel", ImVec2(dimension.x, dimension.y));
//
//	//===================================================================================
//
//	//Partition("MyriadPro_Bold_16", "Plate details", yellow);
//
//	//ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
//
//	//ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
//	//ImGui::Text(("Name: " + plateData.name).c_str());
//
//	//auto maxDimension = Plate::GetMaxDimension();
//
//	//We need to convert the NDC dimensions back to mm for display
//	//auto width = plateData.dimensionMM.x; 
//	//auto height = plateData.dimensionMM.y; 
//
//	//ImGui::Text(("Dimension: " + std::to_string(width) + "mm x " + std::to_string(height) + "mm").c_str());
//
//	//ImGui::PopFont();
//
//	//===================================================================================
//
//	//Spacing(2);
//	Partition("MyriadPro_Bold_16", "Plate details", yellow);
//
//	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
//	std::string vehicle = plateData.isCar ? "car" : "motorbike";
//
//	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
//	
//	//Max characters is mentioned but what about spaces?
//	ImGui::Text(("- This is a " + vehicle + " plate and is allowed a maximum of " + std::to_string(plateData.maxAllowedCharacters) + " characters.").c_str());
//
//	if (plateData.isLegal)
//	{
//		ImGui::Text("- This plate is legal, as long as the correct dimensions are adhered to.");
//	}
//
//	else
//	{
//		ImGui::Text("- This is not a legal plate, but may be used as signage.");
//	}
//
//	std::string sidebadgeAllowed = plateData.isSideBadgeAllowed ? "is" : "is not";
//	ImGui::Text(("- A side badge " + sidebadgeAllowed + " allowed on this plate.").c_str());
//
//	std::string maxLine = plateData.isTwoLineRegistration ? "two-line" : "one-line";
//	ImGui::Text(("- This is a " + maxLine + " registration plate.").c_str());
//
//	//We have to make this panel higher so that all of the text below can be displayed
//	ImGui::Text("- Character height: 79mm");
//	ImGui::Text("- Wide character width: 50mm");
//	ImGui::Text("- Narrow character width: 14mm");
//	
//	//We have to hook in the numerical values here
//	ImGui::Text("- Padding between registration characters: 11mm");
//	ImGui::Text("- Padding between registration and top of plate: ");
//	ImGui::Text("- Padding between registration and bottom of plate: ");
//	ImGui::Text("- Padding between dealer and border: ");
//	ImGui::Text("- Padding between postcode and border: ");
//	ImGui::Text("- Padding between dealer and postcode: ");
//	ImGui::Text("- Padding between BSAU and border: ");
//
//	ImGui::PopFont();
//
//	//===================================================================================
//
//	Spacing(2);
//	Partition("MyriadPro_Bold_16", "Warnings", yellow);
//
//	ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
//
//	if (!isLegal)
//	{
//		ImGui::GetStyle().Colors[ImGuiCol_Text] = red;
//		ImGui::Text("This plate is currently not legal. Please check specifications. Refer to [Help] for more information.");
//	}
//
//	else
//	{
//		ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
//		ImGui::Text("None");
//	}
//
//	ImGui::PopFont();
//
//	//===================================================================================
//
//	ImGui::End();
//}