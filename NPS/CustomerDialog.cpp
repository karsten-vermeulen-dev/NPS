#include "CustomerDialog.h"

//======================================================================================================
CustomerDialog::CustomerDialog()
{
	customers.push_back({ "John Smith",
							"John's Amazing Plates",
							"john@plates.com",
							"1234",
							"1234567890",
							"21 July 2024" });

	customers.push_back({ "Mary Sue",
							"Lamb Inc.",
							"mary@contrary.com",
							"0001",
							"0762534876",
							"9 March 2023" });

	customers.push_back({ "James Bond",
							"Spyware plates",
							"jamesbond@spy.co.uk",
							"5678",
							"9876543210",
							"13 November 2022" });

	customers.push_back({ "Susan Summers",
							"Plates-R-Us",
							"susan@virginmedia.com",
							"2123",
							"2817026543",
							"22 April 2024" });

	customers.push_back({ "Ivan the Terrible",
							"Horrible Plates",
							"ivan@nogood.com",
							"9999",
							"2947265420",
							"1 August 2023" });

	customers.push_back({ "Jessie James",
							"JJ Plates Co.",
							"jesse_james_97@gmail.com",
							"1093",
							"1289735460",
							"10 January 2025" });
}
//======================================================================================================
void CustomerDialog::Show()
{
	ImGui::GetStyle().WindowRounding = 10.0f;
	
	ImGui::Begin("CustomerDialog information",
		nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);

	ImGui::SetWindowSize("CustomerDialog information", ImVec2(dimension.x, dimension.y));

	//===================================================================================

	static int customerID = 0;
	const auto buttonIndent = 138;
	const auto totalCustomers = 6;
	const auto spacingToButton = 5;
	const auto spacingFromTitle = 5;
	const auto spacingBetweenSections = 2;
	const char* customersTemp[totalCustomers];

	auto count = 0;

	for (auto& customer : customers)
	{
		customersTemp[count++] = customer.fullName.c_str();
	}

	//===================================================================================

	Partition("MyriadPro_Bold_16", "Customer information", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::Text("Generate new licence keys for selected customers.");

	Spacing(spacingFromTitle);
	Partition("MyriadPro_Bold_16", "Full name", yellow);
	
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);

	ImGui::Combo(" ", &customerID, customersTemp, totalCustomers);
	
	ImGui::PopFont();

	//===================================================================================

	Spacing(spacingBetweenSections);
	Partition("MyriadPro_Bold_16", "Details", yellow);

	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	ImGui::Text(("Company name: " + customers[customerID].companyName).c_str());

	ImGui::Spacing();
	ImGui::Text(("Site code: " + customers[customerID].siteCode).c_str());

	ImGui::Spacing();
	ImGui::Text(("Email address: " + customers[customerID].emailAddress).c_str());

	ImGui::Spacing();
	ImGui::Text(("Licence key: " + customers[customerID].licenceKey).c_str());

	ImGui::Spacing();
	ImGui::Text(("Expiry date: " + customers[customerID].expiryDate).c_str());
	
	ImGui::PopFont();

	//=================================================================

	Spacing(spacingBetweenSections);
	Partition("MyriadPro_Bold_16", "Renew licence", yellow);

	static bool isEndOfMonth = false;
	
	ImGui::GetStyle().Colors[ImGuiCol_Text] = white;
	ImGui::PushFont(fonts["MyriadPro_Regular"]);
	ImGui::Checkbox("To end of month", &isEndOfMonth);

	static int dayID = 0;
	const auto TOTAL_DAYS = 31;
	const char* days[] = { "1", "2", "3", "4", "5", "6", "7",
						   "8", "9", "10", "11", "12", "13", "14",
						   "15", "16", "17", "18", "19", "20", "21",
						   "22", "23", "24", "25", "26", "27", "28",
						   "29", "30", "31" };

	ImGui::Combo("Day", &dayID, days, TOTAL_DAYS);

	static int monthID = 0;
	const auto TOTAL_MONTHS = 12;
	const char* months[] = { "January", "February", "March",
							 "April", "May", "June",
							 "July", "August", "September",
							 "October", "November", "December" };

	ImGui::Combo("Month", &monthID, months, TOTAL_MONTHS);

	static int yearID = 0;
	const auto TOTAL_YEARS = 10;
	const char* years[] = { "2022", "2023",
							"2024", "2025",
							"2026", "2027",
							"2028", "2029",
							"2030", "2031" };

	ImGui::Combo("Year", &yearID, years, TOTAL_YEARS);

	static int periodID = 0;
	const auto TOTAL_PERIODS = 6;
	const char* periods[] = { "1", "2", "3", "4", "5", "6" };

	ImGui::Combo("Period (months)", &periodID, periods, TOTAL_PERIODS);

	Spacing(spacingToButton);
	ImGui::Indent(buttonIndent);

	if (ImGui::Button("Generate key", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		customers[customerID].licenceKey = "0987654321";
	}

	ImGui::Spacing();
	ImGui::Separator();

	Spacing(spacingToButton);

	if (ImGui::Button("Okay", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}

	if (ImGui::Button("Cancel", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}

	if (ImGui::Button("Help", ImVec2(buttonDimension.x, buttonDimension.y)))
	{
		isVisible = false;
	}

	ImGui::PopFont();

	//=================================================================

	ImGui::End();
}