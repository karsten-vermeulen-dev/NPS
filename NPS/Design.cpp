#include <afxwin.h>
#include <afxext.h>
#include <Windows.h>

#include <fstream>
#include <filesystem>
#include <functional>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "Design.h"
#include "Input.h"
#include "Screen.h"
#include "Tutorial.h"
#include "Utility.h"

//======================================================================================================
bool Design::OnEnter()
{
	mainShader = std::make_unique<Shader>();

	if (!mainShader->Create("Shaders/Main.vert", "Shaders/Main.frag"))
	{
		return false;
	}

	mainShader->BindAttribute("vertexIn");
	mainShader->BindAttribute("colorIn");
	mainShader->BindAttribute("textureIn");

	mainShader->BindUniform("model");
	mainShader->BindUniform("isText");
	mainShader->BindUniform("isTextured");
	mainShader->BindUniform("textureImage");

	mainShader->Use();

	//===============================================================================

	lightShader = std::make_unique<Shader>();

	if (!lightShader->Create("Shaders/Light.vert", "Shaders/Light.frag"))
	{
		return false;
	}

	lightShader->BindAttribute("vertexIn");
	lightShader->BindAttribute("colorIn");
	lightShader->BindAttribute("normalIn");
	lightShader->BindAttribute("textureIn");

	lightShader->BindUniform("model");
	lightShader->BindUniform("normal");
	lightShader->BindUniform("projection");
	lightShader->BindUniform("isTextured");
	lightShader->BindUniform("textureImage");
	lightShader->BindUniform("cameraPosition");

	lightShader->BindUniform("light.ambient");
	lightShader->BindUniform("light.diffuse");
	lightShader->BindUniform("light.specular");
	lightShader->BindUniform("light.position");

	lightShader->BindUniform("material.ambient");
	lightShader->BindUniform("material.diffuse");
	lightShader->BindUniform("material.specular");
	lightShader->BindUniform("material.shininess");

	//===================================================================

	const auto menuBarHeight = 19;
	const auto minorWidth = 0.2f;
	const auto minorHeight = 0.255f;
	const auto majorWidth = 1.0f - minorWidth;
	const auto majorHeight = 1.0f - minorHeight;

	const auto mainResolution = static_cast<glm::vec2>(Screen::Instance()->GetMainResolution());

	Screen::Instance()->SetViewport((GLint)(minorWidth * mainResolution.x),
		(GLint)(minorHeight * mainResolution.y),
		(GLsizei)(majorWidth * mainResolution.x),
		(GLsizei)(majorHeight * mainResolution.y - menuBarHeight));

	//===================================================================

	paper = std::make_unique<Image>("Assets/Images/Paper.png");
	backdrop = std::make_unique<Image>("Assets/Images/Backdrop.png");

	//===================================================================

	//The maximum width of the viewport is 650mm, 
	//The height will adjust based on aspect ratio
	Plate::Initialize("Data/Plates.inf", 650.0f);

	//===================================================================

	//The height of the text is multiplied by 1.37 because 
	//the resulting pixel size is not always as we expect
	const auto scale = 1.37f;

	//Text sizes are between 3mm and 10mm
	Text::Load("Assets/Fonts/Arial_bold.ttf", 10.0f * scale, "Arial_bold_10mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 8.0f * scale, "Arial_bold_8mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 7.0f * scale, "Arial_bold_7mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 6.0f * scale, "Arial_bold_6mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 5.0f * scale, "Arial_bold_5mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 4.0f * scale, "Arial_bold_4mm");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 3.0f * scale, "Arial_bold_3mm");

	//===================================================================

	Registration::Load("Assets/Images/Fonts/2D_car", "Car_2D");
	Registration::Load("Assets/Images/Fonts/3D_car_under", "Car_3D");
	Registration::Load("Assets/Images/Fonts/4D_car_under", "Car_4D");

	Registration::Load("Assets/Images/Fonts/2D_motorcycle", "Bike_2D");
	Registration::Load("Assets/Images/Fonts/3D_motorcycle_under", "Bike_3D");
	Registration::Load("Assets/Images/Fonts/4D_motorcycle_under", "Bike_4D");

	//===================================================================

	//We should be loading all main assets ONCE for the
	//entire program and releasing them when the app is done
	//We have implemented a temporary solution here 
	static bool isLoaded = false;

	if (!isLoaded)
	{
		//Regular MyriadPro fonts
		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_regular.otf", 14.0f, "MyriadPro_Regular_14"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_regular.otf", 16.0f, "MyriadPro_Regular_16"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_regular.otf", 24.0f, "MyriadPro_Regular_24"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_regular.otf", 32.0f, "MyriadPro_Regular_32"))
		{
			return false;
		}

		//Bold MyriadPro fonts
		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_bold.otf", 14.0f, "MyriadPro_Bold_14"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_bold.otf", 16.0f, "MyriadPro_Bold_16"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_bold.otf", 24.0f, "MyriadPro_Bold_24"))
		{
			return false;
		}

		if (!UserInterface::LoadFont("Assets/Fonts/MyriadPro_bold.otf", 32.0f, "MyriadPro_Bold_32"))
		{
			return false;
		}

		isLoaded = true;
	}

	//===================================================================

	//Dimensions of UI must adjust based on app's resolution

	const auto buttonDimension = glm::uvec2(100, 22);

	mainMenu = std::make_unique<MainMenu>();
	mainMenu->IsVisible(true);

	fontDialog = std::make_unique<FontDialog>("Data/Fonts.ini");
	fontDialog->IsVisible(false);
	fontDialog->SetDimension(glm::uvec2(325, 580));
	fontDialog->SetButtonDimension(buttonDimension);

	aboutDialog = std::make_unique<AboutDialog>();
	aboutDialog->IsVisible(false);
	aboutDialog->SetDimension(glm::uvec2(300, 125));
	aboutDialog->SetButtonDimension(buttonDimension);

	messageDialog = std::make_unique<MessageDialog>();
	messageDialog->IsVisible(false);
	messageDialog->SetDimension(glm::uvec2(300, 150));
	messageDialog->SetButtonDimension(buttonDimension);

	activateDialog = std::make_unique<ActivateDialog>();
	activateDialog->IsVisible(false);
	activateDialog->SetDimension(glm::uvec2(325, 410));
	activateDialog->SetButtonDimension(buttonDimension);

	customerDialog = std::make_unique<CustomerDialog>();
	customerDialog->IsVisible(false);
	customerDialog->SetDimension(glm::uvec2(400, 660));
	customerDialog->SetButtonDimension(buttonDimension);

	licenseDialog = std::make_unique<LicenseDialog>();
	licenseDialog->IsVisible(false);
	licenseDialog->SetDimension(glm::uvec2(325, 410));
	licenseDialog->SetButtonDimension(buttonDimension);

	//===================================================================

	const auto UIPadding = 1;

	modePanel = std::make_unique<ModePanel>();
	modePanel->IsVisible(true);
	modePanel->SetPosition(glm::uvec2(mainResolution.x - (0.3f * minorWidth * mainResolution.x) + UIPadding, menuBarHeight));
	modePanel->SetDimension(glm::uvec2(0.3f * minorWidth * mainResolution.x, (0.65f * minorHeight * mainResolution.y) + UIPadding));

	propertiesPanel = std::make_unique<PropertiesPanel>();
	propertiesPanel->IsVisible(true);
	propertiesPanel->SetButtonDimension(buttonDimension);
	propertiesPanel->SetPosition(glm::uvec2(0, menuBarHeight));
	propertiesPanel->SetDimension(glm::uvec2(minorWidth * mainResolution.x, mainResolution.y - menuBarHeight));

	/*feedbackPanel = std::make_unique<FeedbackPanel>();
	feedbackPanel->IsVisible(true);
	feedbackPanel->SetPosition(glm::uvec2(minorWidth * mainResolution.x - UIPadding, majorHeight * mainResolution.y));
	feedbackPanel->SetDimension(glm::uvec2(majorWidth * mainResolution.x - (minorWidth * mainResolution.x) + (UIPadding * 2), minorHeight * mainResolution.y + UIPadding));*/

	printPanel = std::make_unique<PrintPanel>();
	printPanel->IsVisible(true);
	printPanel->SetButtonDimension(buttonDimension);
	printPanel->SetPosition(glm::uvec2(mainResolution.x - (minorWidth * mainResolution.x), majorHeight * mainResolution.y));
	printPanel->SetDimension(glm::uvec2(minorWidth * mainResolution.x, minorHeight * mainResolution.y + UIPadding));

	//===================================================================

	plate = std::make_unique<Plate>("Standard Oblong", propertiesPanel->GetProperties());

	//===================================================================

	ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = UserInterface::yellow;
	ImGui::GetStyle().Colors[ImGuiCol_Separator] = UserInterface::darkGrey;

	ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = UserInterface::darkGrey;
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = UserInterface::lightGrey;
	ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = UserInterface::lightGrey;

	ImGui::GetStyle().Colors[ImGuiCol_Border] = UserInterface::yellow;
	ImGui::GetStyle().Colors[ImGuiCol_Button] = UserInterface::darkGrey;

	ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = UserInterface::lightGrey;
	ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = UserInterface::yellow;

	ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = UserInterface::yellow;
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = UserInterface::yellow;

	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = UserInterface::lightGrey;

	ImGui::GetStyle().ItemSpacing = ImVec2(5, 5);
	ImGui::GetStyle().FrameRounding = 5.0f;
	ImGui::GetStyle().GrabRounding = 5.0f;

	//Rafal Guz's formula for correctly scaling the window
	//Fullscreen resolution could be a standard 1920x1080
	//If the width is decreased or the height increased the ratio gets smaller but
	//we don't care about height increase only the fractional value to scale down by.
	//if (resolution.x / resolution.y < fullscreenResolution.x / fullscreenResolution.y)
	//{
	//	scaleFactor = resolution.x / fullscreenResolution.x;
	//}
	//
	//If the width is increased or the height decreased the ratio gets larger but
	//we don't care about width increase only the fractional value to scale down by.
	//else
	//{
	//	scaleFactor = resolution.y / fullscreenResolution.y;
	//}

	mainMenu->IsHost(isHost);
	mainMenu->IsActivated(isActivated);

	return true;
}
//======================================================================================================
State* Design::Update()
{
	if (Input::Instance()->IsXClicked() || isStateComplete)
	{
		return nullptr;
	}

	return this;
}
//======================================================================================================
bool Design::Render()
{
	static bool isPrintModeLoaded = false;
	static bool is3DViewModeLoaded = false;

	Screen::Instance()->Refresh();

	if (modePanel->GetMode().isDesign)
	{
		isPrintModeLoaded = false;
		is3DViewModeLoaded = false;

		printPanel->IsActive(false);
		propertiesPanel->IsActive(true);

		mainShader->Use();
		Screen::Instance()->CreateNDCView();

		backdrop->Render(*mainShader);
		plate->Render(*mainShader);

		//Licence expiration
		const int licenceDay = 31;        
		const int licenceMonth = 12;      
		const int licenceYear = 2022;     

		//Expiry date struct
		std::tm expiryDate{};
		expiryDate.tm_year = licenceYear - 1900;  
		expiryDate.tm_mon = licenceMonth - 1;     
		expiryDate.tm_mday = licenceDay;          

		std::time_t currentTime = time(0);
		std::time_t licence = std::mktime(&expiryDate);
		//std::cout << "LICENCE : " << std::asctime(std::localtime(&licence));

		//Check licence
		/*if (licence < currentTime)
		{
			messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
			messageDialog->SetTitle("Licence error");
			messageDialog->SetMessage("Licence expired. Please renew now.");
			messageDialog->IsVisible(true);
		}*/
	}

	else if (modePanel->GetMode().isPrint)
	{
		is3DViewModeLoaded = false;

		auto maxDimension = Plate::GetMaxDimension();

		if (!isPrintModeLoaded)
		{
			//We first need to create a directory to store the plate image files
			//This function should be hidden in the lower levels and the name of the 
			//directory should be a global setting, possibly read in from a file?
			std::filesystem::create_directories("Plates");

			//Temp. disable - Bug here!
			//Create a 24-bit image for the printer (Windows requires this)
			//Here, we are saving the image to print later, we don't want the yellow plate
			//SaveToFile("Plates/Plate_print.bmp", 203, false, true);

			//Temp. disable - Bug here!
			//Create a 32-bit image for preview
			//Here, we are saving the image to print preview later, we don't want the yellow plate
			//SaveToFile("Plates/Plate_print_preview.png", 203, true, true);

			plateToPrint.release();
			plateToPrint = std::make_unique<Image>("Plates/Plate_print_preview.png");

			//We add on 5mm of extra printing paper space
			auto extraSpace = glm::vec2(2.0f * (5.0f / maxDimension.x), 2.0f * (5.0f / maxDimension.y));
			
			auto dimensionNDC = glm::vec2(0.0f);
			dimensionNDC.x = Utility::ConvertToNDC(plate->GetProperties().plateWidth, maxDimension.x);
			dimensionNDC.y = Utility::ConvertToNDC(plate->GetProperties().plateHeight, maxDimension.y);

			plateToPrint->SetDimension(dimensionNDC.x, dimensionNDC.y);
			paper->SetDimension(dimensionNDC.x + extraSpace.x, dimensionNDC.y + extraSpace.y);

			isPrintModeLoaded = true;
		}

		printPanel->IsActive(true);
		propertiesPanel->IsActive(false);

		mainShader->Use();
		Screen::Instance()->CreateNDCView();

		backdrop->Render(*mainShader);
		paper->Render(*mainShader);

		auto position = glm::vec3(0.0f);

		position.x += 2.0f * (printPanel->GetMetrics().leftMargin / maxDimension.x);
		position.y -= 2.0f * (printPanel->GetMetrics().topMargin / maxDimension.y);

		plateToPrint->GetTransform().SetPosition(position);
		plateToPrint->Render(*mainShader);
	}

	else if (modePanel->GetMode().is3DView)
	{
		isPrintModeLoaded = false;

		if (!is3DViewModeLoaded)
		{
			//We first need to create a directory to store the plate image files
			//This function should be hidden in the lower levels and the name of the 
			//directory should be a global setting, possibly read in from a file?
			std::filesystem::create_directories("Plates");

			//Temp. disable - Bug here!
			//Here, we are not printing, we only save the file to preview later
			//SaveToFile("Plates/Plate_3D_view.png", 203, true, false);

			//auto dimension = plate->GetPlateData().dimensionNDC;
			auto dimension = plate->GetLegalDimensionNDC();

			plateToPrint.release();
			plateToPrint = std::make_unique<Image>("Plates/Plate_3D_view.png");
			plateToPrint->SetDimension(dimension.x, dimension.y);
			plateToPrint->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));

			is3DViewModeLoaded = true;
		}

		printPanel->IsActive(false);
		propertiesPanel->IsActive(false);

		lightShader->Use();
		Screen::Instance()->CreatePerspView(*lightShader);

		plateToPrint->Update();
		plateToPrint->Render(*lightShader, true);
	}

	//==============================================================================
	//ImGUI UI
	//==============================================================================

	//For help with win32 dialog boxes
	//https://docs.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes#opening-a-file

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::GetStyle().Colors[ImGuiCol_Text] = UserInterface::white;

	mainMenu->Show();

	auto& menuItems = mainMenu->GetMenuItems();

	if (menuItems.isNewSelected)
	{
		messageDialog->SetTag("New");
		messageDialog->SetButtonType(MessageDialog::ButtonType::YesNo);
		messageDialog->SetTitle("New plate");
		messageDialog->SetMessage("Are you sure you want to create a new design?");
		messageDialog->IsVisible(true);
	}

	else if (menuItems.isLoadSelected)
	{
		//We use a standard Windows 'Open' dialog box because ImGui doesn't have it's own
		auto filename = Utility::WindowsOpenFile(L"All Number Plate Files\0*.nps\0");

		if(!filename.empty())
		{
			std::map<std::string, std::string> dataMap;

			Utility::LoadConfigFile(filename, dataMap);

			auto& plateProperties = propertiesPanel->GetProperties();
			
			plate->SetPlateData(dataMap["Plate"]);

			plateProperties.plateWidth = std::stoi(dataMap["PlateWidth"]);
			plateProperties.plateHeight = std::stoi(dataMap["PlateHeight"]);

			if (dataMap["FontType"] == "Car")
			{
				fontDialog->fontType.isCar = true;
				fontDialog->fontType.isMotorCycle = false;
				fontDialog->fontType.isCustom = false;
			}

			else if (dataMap["FontType"] == "Motorcycle")
			{
				fontDialog->fontType.isMotorCycle = true;
				fontDialog->fontType.isCar = false;
				fontDialog->fontType.isCustom = false;
			}

			else if (dataMap["FontType"] == "Custom")
			{
				fontDialog->fontType.isCustom = true;
				fontDialog->fontType.isCar = false;
				fontDialog->fontType.isMotorCycle = false;
			}

			if (dataMap["FontStyle"] == "2DRegular")
			{
				fontDialog->fontStyle = FontDialog::FontStyle::Regular2D;
			}
			
			else if (dataMap["FontStyle"] == "3DGelResin")
			{
				fontDialog->fontStyle = FontDialog::FontStyle::GelResin3D;
			}
			
			else if (dataMap["FontStyle"] == "4DLaserCut")
			{
				fontDialog->fontStyle = FontDialog::FontStyle::Lasercut4D;
			}

			plateProperties.raisedRegistration = std::stoi(dataMap["RegTextRaise"]);
			plateProperties.nudgedRegistration = std::stoi(dataMap["RegTextNudge"]);

			plateProperties.raisedTwoLineSpace = std::stoi(dataMap["RegTwoLineSpaceRaise"]);
			plateProperties.registrationText = dataMap["RegText"];

			plateProperties.isBorderVisible = std::stoi(dataMap["BorderVisible"]);
			plateProperties.isSideBadgeVisible = std::stoi(dataMap["BorderSideBadge"]);

			plateProperties.borderSize = std::stoi(dataMap["BorderSize"]);
			plateProperties.marginSize = std::stoi(dataMap["MarginSize"]);

			plateProperties.dealerText = dataMap["DealerText"];
			plateProperties.isDealerVisible = std::stoi(dataMap["DealerTextVisible"]);
			plateProperties.isDealerAbovePostcode = std::stoi(dataMap["DealerTextAbovePostcode"]);

			plateProperties.raisedDealer = std::stoi(dataMap["DealerTextRaise"]);
			plateProperties.nudgedDealer = std::stoi(dataMap["DealerTextNudge"]);

			plateProperties.postcodeText = dataMap["PostcodeText"];
			plateProperties.raisedPostcode = std::stoi(dataMap["PostcodeTextRaise"]);
			plateProperties.nudgedPostcode = std::stoi(dataMap["PostcodeTextNudge"]);

			plateProperties.BSAUText = dataMap["BSAUText"];
			plateProperties.isBSAUVisible  = std::stoi(dataMap["BSAUVisible"]); 
			plateProperties.isBSAUOnBorder = std::stoi(dataMap["BSAUOnBorder"]);
			plateProperties.raisedBSAU = std::stoi(dataMap["BSAUTextRaise"]);
			plateProperties.nudgedBSAU = std::stoi(dataMap["BSAUTextNudge"]);
		}
	}

	else if (menuItems.isSaveAsSelected)
	{
		//We use a standard Windows 'Save As' dialog box because ImGui doesn't have it's own
		auto filename = Utility::WindowsSaveFile(L"All Number Plate Files\0*.nps\0");
		
		if (!filename.empty())
		{
			std::map<std::string, std::string> dataMap;

			auto plateProperties = propertiesPanel->GetProperties();

			dataMap["Plate"] = plateProperties.plateName;
			dataMap["PlateWidth"] = std::to_string(plateProperties.plateWidth);
			dataMap["PlateHeight"] = std::to_string(plateProperties.plateHeight);

			if (fontDialog->fontType.isCar)
			{
				dataMap["FontType"] = "Car";
			}

			else if(fontDialog->fontType.isMotorCycle)
			{
				dataMap["FontType"] = "Motorcycle";
			}

			else if (fontDialog->fontType.isCustom)
			{
				dataMap["FontType"] = "Custom";
			}

			//-------------------------------------------------------

			if (fontDialog->fontStyle == FontDialog::FontStyle::Regular2D)
			{
				dataMap["FontStyle"] = "2DRegular";
			}

			else if (fontDialog->fontStyle == FontDialog::FontStyle::GelResin3D)
			{
				dataMap["FontStyle"] = "3DGelResin";
			}

			else if (fontDialog->fontStyle == FontDialog::FontStyle::Lasercut4D)
			{
				dataMap["FontStyle"] = "4DLaserCut";
			}

			//-------------------------------------------------------
			
			dataMap["RegTextRaise"] = std::to_string(plateProperties.raisedRegistration);
			dataMap["RegTextNudge"] = std::to_string(plateProperties.nudgedRegistration);
			dataMap["RegTwoLineSpaceRaise"] = std::to_string(plateProperties.raisedTwoLineSpace);
			dataMap["RegText"] = plateProperties.registrationText;

			dataMap["BorderVisible"] = (plateProperties.isBorderVisible) ? "1" : "0";
			dataMap["BorderSideBadge"] = (plateProperties.isSideBadgeVisible) ? "1" : "0";
			
			dataMap["BorderSize"] = std::to_string(plateProperties.borderSize);
			dataMap["MarginSize"] = std::to_string(plateProperties.marginSize);
			
			dataMap["DealerText"] = plateProperties.dealerText;
			dataMap["DealerTextVisible"] = (plateProperties.isDealerVisible) ? "1" : "0";
			dataMap["DealerTextAbovePostcode"] = (plateProperties.isDealerAbovePostcode) ? "1" : "0";
			dataMap["DealerTextRaise"] = std::to_string(plateProperties.raisedDealer);
			dataMap["DealerTextNudge"] = std::to_string(plateProperties.nudgedDealer);

			dataMap["PostcodeText"] = plateProperties.postcodeText;
			dataMap["PostcodeTextRaise"] = std::to_string(plateProperties.raisedPostcode);
			dataMap["PostcodeTextNudge"] = std::to_string(plateProperties.nudgedPostcode);

			dataMap["BSAUText"] = plateProperties.BSAUText;
			dataMap["BSAUVisible"] = (plateProperties.isBSAUVisible) ? "1" : "0";
			dataMap["BSAUOnBorder"] = (plateProperties.isBSAUOnBorder) ? "1" : "0";
			dataMap["BSAUTextRaise"] = std::to_string(plateProperties.raisedBSAU);
			dataMap["BSAUTextNudge"] = std::to_string(plateProperties.nudgedBSAU);

			Utility::SaveConfigFile(filename, dataMap);
		}
	}

	else if (menuItems.isExitSelected)
	{
		messageDialog->SetTag("Exit");
		messageDialog->SetButtonType(MessageDialog::ButtonType::YesNo);
		messageDialog->SetTitle("Exit application");
		messageDialog->SetMessage("Are you sure you want to exit?");
		messageDialog->IsVisible(true);
	}

	else if (menuItems.isStandardOblongSelected)
	{
		plate->SetPlateData("Standard Oblong");
	}

	else if (menuItems.isShort13InchSelected)
	{
		plate->SetPlateData("Short 13\"");
	}

	else if (menuItems.isShort16InchSelected)
	{
		plate->SetPlateData("Short 16\"");
	}

	else if (menuItems.isShorter17InchSelected)
	{
		plate->SetPlateData("Shorter 17\"");
	}

	else if (menuItems.isShorter18InchSelected)
	{
		plate->SetPlateData("Shorter 18\"");
	}

	else if (menuItems.isOversizeOblongSelected)
	{
		plate->SetPlateData("Oversize Oblong");
	}

	else if (menuItems.isShapedPlateSelected)
	{
		plate->SetPlateData("Shaped Plate");
	}

	else if (menuItems.isMotorcycle7x5InchSelected)
	{
		plate->SetPlateData("Motorcycle 7x5\"");
	}

	else if (menuItems.isMotorcycle8x6InchSelected)
	{
		plate->SetPlateData("Motorcycle 8x6\"");
	}

	else if (menuItems.isMotorcycle9x7InchSelected)
	{
		plate->SetPlateData("Motorcycle 9x7\"");
	}

	else if (menuItems.isSquare11x8InchSelected)
	{
		plate->SetPlateData("Square 11x8\"");
	}

	else if (menuItems.isUSAImport12x6InchSelected)
	{
		plate->SetPlateData("USA Import 12x6\"");
	}

	else if (menuItems.isLargeEuroSquare12x8InchSelected)
	{
		plate->SetPlateData("Large Euro Square");
	}

	else if (menuItems.isImport13x6Point5InchSelected)
	{
		plate->SetPlateData("Import 13x6.5\"");
	}

	else if (menuItems.isImport13x7InchSelected)
	{
		plate->SetPlateData("Import 13x7\"");
	}

	else if (menuItems.isMicroPlate9x3InchSelected)
	{
		plate->SetPlateData("Micro Plate 9x3\"");
	}

	else if (menuItems.isMicroPlate12x3InchSelected)
	{
		plate->SetPlateData("Micro Plate 12x3\"");
	}

	else if (menuItems.isFontSettingsSelected)
	{
		fontDialog->IsVisible(true);
	}

	else if (menuItems.isTutorialSelected)
	{
		newState = new Tutorial;
	}

	else if (menuItems.isAboutSelected)
	{
		aboutDialog->IsVisible(true);
	}

	else if (menuItems.isActivateProgramSelected)
	{
		activateDialog->IsVisible(true);
	}

	else if (menuItems.isCustomerInformationSelected)
	{
		customerDialog->IsVisible(true);
	}

	else if (menuItems.isCheckLicenseSelected)
	{
		licenseDialog->IsVisible(true);
	}

	//========================================================================================

	propertiesPanel->Show();
	auto& plateProperties = propertiesPanel->GetProperties();

	//feedbackPanel->SetLegality(plate->IsLegal());
	//feedbackPanel->SetPlateData(plate->GetPlateData());
	//feedbackPanel->Show();

	modePanel->Show();

	printPanel->Show();

	if (printPanel->GetButtonState().print)
	{
		PrintPlate();
	}

	if (fontDialog->IsVisible())
	{
		fontDialog->Show();

		//Load default fonts for car and motorcycle registration (2D/3D/4D)
		if (fontDialog->fontToChange.isRegistration && 
			(fontDialog->fontType.isCar || fontDialog->fontType.isMotorCycle))
		{
			plate->LoadDefaultFont(fontDialog->fontType, fontDialog->fontStyle);
		}

		//Load custom font for registration, dealer, postcode or BSAU
		else
		{
			//Path and filename of requested font
			auto customFont = fontDialog->GetFont();

			if (!customFont.empty())
			{
				if (fontDialog->fontToChange.isRegistration)
				{
					plate->LoadCustomFont(Plate::FontToChange::Registration, customFont, 100);
				}

				else if (fontDialog->fontToChange.isDealer)
				{
					plate->LoadCustomFont(Plate::FontToChange::Dealer, customFont, 10);
				}

				else if (fontDialog->fontToChange.isPostcode)
				{
					plate->LoadCustomFont(Plate::FontToChange::Postcode, customFont, 10);
				}

				else if (fontDialog->fontToChange.isBSAU)
				{
					plate->LoadCustomFont(Plate::FontToChange::BSAU, customFont, 8);
				}
			}
		}
	}

	if (aboutDialog->IsVisible())
	{
		aboutDialog->Show();
	}

	else if (messageDialog->IsVisible())
	{
		//At this stage we want to grey out the background 
		//and only display/activate the message box
		messageDialog->Show();

		//Internal class code?
		//--------------------------------------------------
		if (messageDialog->GetTag() == "New" && messageDialog->GetButtonState().yes)
		{
			ResetView();
			messageDialog->IsVisible(false);
		}

		else if (messageDialog->GetTag() == "Exit" && messageDialog->GetButtonState().yes)
		{
			isStateComplete = true;
		}

		else if (messageDialog->GetButtonState().no
			|| messageDialog->GetButtonState().ok
			|| messageDialog->GetButtonState().cancel)
		{
			messageDialog->IsVisible(false);
		}
		//--------------------------------------------------
	}

	else if (activateDialog->IsVisible())
	{
		activateDialog->Show();
	}

	else if (customerDialog->IsVisible())
	{
		customerDialog->Show();
	}

	else if (licenseDialog->IsVisible())
	{
		licenseDialog->Show();
	}

	plate->SetProperties();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}
//======================================================================================================
void Design::OnExit()
{
	Text::Unload("Arial_bold_10mm");
	Text::Unload("Arial_bold_8mm");
	Text::Unload("Arial_bold_6mm");
	Text::Unload("Arial_bold_4mm");

	Registration::Unload("Car_4D");
	Registration::Unload("Car_3D");
	Registration::Unload("Car_2D");

	Registration::Unload("Bike_4D");
	Registration::Unload("Bike_3D");
	Registration::Unload("Bike_2D");

	//m_mainShader->Destroy();
	//Text::Shutdown();
	//Texture::Unload();
	//Shader::Shutdown();
	//Screen::Instance()->DeactivateContext();
	//Screen::Instance()->Shutdown();

	//Perhaps there is a better way to do this?
	const auto mainResolution = Screen::Instance()->GetMainResolution();
	Screen::Instance()->SetViewport(0, 0, mainResolution.x, mainResolution.y);
	Screen::Instance()->Refresh();
}
//======================================================================================================
void Design::SaveToFile(const std::string& filename, int DPI, bool hasAlphaChannel, bool isPrinting)
{
	//This will be in NDC coordinates
	//auto plateDimension = plate->GetPlateData().dimensionNDC;
	//auto plateDimension = plate->GetLegalDimensionNDC();

	//The max dimension of the viewport in mm
	auto maxDimension = Plate::GetMaxDimension();

	auto plateDimensionNDC = glm::vec2(0.0f);
	plateDimensionNDC.x = Utility::ConvertToNDC(plate->GetProperties().plateWidth, maxDimension.x);
	plateDimensionNDC.y = Utility::ConvertToNDC(plate->GetProperties().plateHeight, maxDimension.y);

	//convert plate dimension back to mm
	//auto mm = glm::vec2(0.5f * plateDimension.x * maxDimension.x, 0.5f * plateDimension.y * maxDimension.y);
	auto mm = glm::vec2(0.5f * plateDimensionNDC.x * maxDimension.x, 0.5f * plateDimensionNDC.y * maxDimension.y);

	//Then convert mm to inches
	auto inches = glm::highp_dvec2(mm.x * 0.03937007874, mm.y * 0.03937007874);

	//The full framebuffer/image size based on printer's DPI
	//This will be the resolution of the image stored on disk
	auto fullSize = inches * static_cast<double>(DPI);

	//==================================================================================
	//Could we place the plate saving code into the Plate class?
	//Create frame buffer

	//We need to store current resolution so that we can set it back again
	auto viewport = Screen::Instance()->GetViewport();

	//auto& mainShader = *mainShader;
	mainShader->Use();

	//Create a new framebuffer 
	Screen::Instance()->CreateFrameBuffer(fullSize);

	//We need to specifically target a large rendering
	//area which is larger than our native resolution
	Screen::Instance()->SetViewport(0, 0, (GLsizei)fullSize.x, (GLsizei)fullSize.y);

	//Set backround color to white because we print on white backgrounds
	Screen::Instance()->SetColor(1.0f, 1.0f, 1.0f, (hasAlphaChannel) ? 0.0f : 1.0f);
	Screen::Instance()->Refresh();

	Plate::Print(*plate, propertiesPanel->GetProperties(), glm::vec2(mm.x, mm.y), *mainShader, isPrinting);

	//This will render to our currently bound custom frame buffer
	Screen::Instance()->Present();

	//Save frame buffer image to file
	Screen::Instance()->SaveScreenToFile(fullSize, filename, hasAlphaChannel);

	//After we are done with screen capture make sure to set 
	//the viewing area and frame buffer back to their defaults
	Screen::Instance()->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	Screen::Instance()->DestroyFrameBuffer();

	//Reset display back to normal
	Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.w, viewport.h);
}
//======================================================================================================
void Design::ResetView()
{
	propertiesPanel->Reset();
	fontDialog->Reset(); //we reset but we dont apply to the plate yet
}
//======================================================================================================
void Design::PrintPlate()
{
	//Solutions for printing images acquired here:
	//https://www.codeproject.com/Articles/12391/Print-a-bitmap-full-page
	//https://www.codeproject.com/Tips/5276174/Setting-Landscape-Mode-in-an-MFC-Application-Writt

	PRINTDLG printDialog;

	ZeroMemory(&printDialog, sizeof(printDialog));
	printDialog.lStructSize = sizeof(printDialog);
	printDialog.hwndOwner = Screen::Instance()->GetWindowHandle();
	printDialog.hDevMode = nullptr;
	printDialog.hDevNames = nullptr;
	printDialog.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	printDialog.nCopies = 1;
	printDialog.nFromPage = 0xFFFF;
	printDialog.nToPage = 0xFFFF;
	printDialog.nMinPage = 1;
	printDialog.nMaxPage = 0xFFFF;

	if (PrintDlg(&printDialog) == TRUE)
	{
		if (!printDialog.hDC)
		{
			messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
			messageDialog->SetTitle("Printer error");
			messageDialog->SetMessage("No printer found.");
			messageDialog->IsVisible(true);
		}

		else
		{
			CDC deviceContext;

			if (!deviceContext.Attach(printDialog.hDC))
			{
				messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
				messageDialog->SetTitle("Printer error");
				messageDialog->SetMessage("No printer found.");
				messageDialog->IsVisible(true);
			}

			//Get the DPI value of the current printer we are using
			//This is used to create the pre-rendered image for printing
			//We could also use a hard-coded value here (high = better)
			auto DPI = deviceContext.GetDeviceCaps(LOGPIXELSX);

			//Fix the below issue
			//We don't want to save the image to file when in 'Print' state
			//but we need the correct DPI when saving the file and the DPI
			//can only be determined after using the print dialog box ??
			//SaveToFile(DPI);

			//===================================================================================

			DOCINFO documentInfo;
			ZeroMemory(&documentInfo, sizeof(DOCINFO));
			documentInfo.cbSize = sizeof(DOCINFO);
			documentInfo.lpszDocName = L"Plates/Plate_print.bmp";

			//Begin a new print job 
			if (deviceContext.StartDocW(&documentInfo))
			{
				deviceContext.StartPage();

				CBitmap bitmap;

				auto image = LoadImage(GetModuleHandle(NULL),
					documentInfo.lpszDocName,
					IMAGE_BITMAP,
					0,
					0,
					LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

				if (!image)
				{
					messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
					messageDialog->SetTitle("Image error");
					messageDialog->SetMessage("Error loading image.");
					messageDialog->IsVisible(true);
				}

				else if (!bitmap.Attach(image))
				{
					messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
					messageDialog->SetTitle("Image error");
					messageDialog->SetMessage("Error processing image.");
					messageDialog->IsVisible(true);
				}

				else
				{
					BITMAP bm;
					bitmap.GetBitmap(&bm);
					auto width = bm.bmWidth;
					auto height = bm.bmHeight;

					//Figure out the correct destination resolution for the printing page
					//What dimensions are these - pixels, mm ??
					//auto pageWidth = deviceContext.GetDeviceCaps(HORZRES);
					//auto pageHeight = deviceContext.GetDeviceCaps(VERTRES);

					auto pageWidth = width;
					auto pageHeight = height;

					//Create memory device context
					CDC memoryDeviceContext;
					memoryDeviceContext.CreateCompatibleDC(&deviceContext);

					auto bitmapHandle = memoryDeviceContext.SelectObject(&bitmap);
					memoryDeviceContext.SetMapMode(deviceContext.GetMapMode());
					deviceContext.SetStretchBltMode(HALFTONE);

					//Render loaded image to the printer in memory
					deviceContext.StretchBlt(
						printPanel->GetMetrics().leftMargin,
						printPanel->GetMetrics().topMargin,
						pageWidth,
						pageHeight,
						&memoryDeviceContext,
						0,
						0,
						width,
						height,
						SRCCOPY);

					//Clean up
					memoryDeviceContext.SelectObject(bitmapHandle);

					//End the printing job successfully or abort it if there was an error
					if (deviceContext.EndPage() > 0)
					{
						deviceContext.EndDoc();
					}

					else
					{
						messageDialog->SetButtonType(MessageDialog::ButtonType::OkCancel);
						messageDialog->SetTitle("Printing error");
						messageDialog->SetMessage("Error printing the image.");
						messageDialog->IsVisible(true);
						deviceContext.AbortDoc();
					}
				}
			}
		}

		//Free the printer's device context when done
		DeleteDC(printDialog.hDC);

		//===================================================================================================================================================
		//For reference - do not remove yet!

		//auto r = StretchBlt(printDialog.hDC,
		//				static_cast<int>(leftOffset),                                 //position image left/right
		//				static_cast<int>(topOffset),								  //position image up/down
		//				static_cast<int>(bitmapDimension.bmWidth + widthOffset),      //target width using pre-rendered image width + adjustment
		//				static_cast<int>(bitmapDimension.bmHeight + heightOffset),    //target height using pre-rendered image height + adjustment
		//				memoryDC, //.GetSafeHdc(),
		//				0,
		//				0,
		//				bitmapDimension.bmWidth,									  //source pre-rendered image width (as-is on file)
		//				bitmapDimension.bmHeight,									  //source pre-rendered image height (as-is on file)
		//				SRCCOPY);
		//===================================================================================================================================================

	}
}