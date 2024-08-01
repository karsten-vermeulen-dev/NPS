#include <algorithm>
#include <vector>
#include "Plate.h"
#include "Screen.h"
#include "Utility.h"

bool Plate::isPrinting{ false };
glm::vec2 Plate::maxDimension{ 0.0f };
std::map<std::string, Plate::PlateData> Plate::plateDataMap;
//================================================================================================
void Plate::Initialize(GLfloat maxWidth)
{
	auto viewport = Screen::Instance()->GetViewport();
	auto aspectRatio = viewport.w / static_cast<float>(viewport.h);

	maxDimension.x = maxWidth;
	maxDimension.y = maxDimension.x / aspectRatio;

	auto NormalizePlate = [](const glm::ivec2& dimensionMM)
	{
		return glm::vec2(2.0f * (dimensionMM.x / maxDimension.x),
			2.0f * (dimensionMM.y / maxDimension.y));
	};

	//Refer to [10] Meeting video for extra help
	//Side badge not allowed
	//Maximum 7 characters + 1 space allowed - legal

	PlateData plateData;

	plateData.name = "Standard Oblong";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 520;
	plateData.dimensionMM.y = 111;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Standard Oblong"] = plateData;

	//============================================================================

	//Maximum 5 characters only (space must be included) - legal

	plateData.name = "Short 13\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 337;
	plateData.dimensionMM.y = 111;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 5;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Short 13 Inch"] = plateData;

	//============================================================================

	//Maximum 6 characters only (space must be included) - legal

	plateData.name = "Short 16\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 405;
	plateData.dimensionMM.y = 111;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 5;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Short 16 Inch"] = plateData;

	//============================================================================

	//Side badge not allowed
	//Maximum 7 characters only (space must be included) - legal

	plateData.name = "Shorter 17\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 431;
	plateData.dimensionMM.y = 111;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 5;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Shorter 17 Inch"] = plateData;

	//============================================================================

	//Side badge not allowed
	//Maximum 5 characters + 1 space allowed (with side badge) - illegal
	//Maximum 7 characters + 1 space allowed (without side badge) - legal

	plateData.name = "Shorter 18\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 455;
	plateData.dimensionMM.y = 111;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 5;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Shorter 18 Inch"] = plateData;

	//============================================================================

	plateData.name = "Oversize Oblong";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 533;
	plateData.dimensionMM.y = 152;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Oversize Oblong"] = plateData;

	//============================================================================

	plateData.name = "Shaped Plate";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 616;
	plateData.dimensionMM.y = 152;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Shaped Plate"] = plateData;

	//============================================================================
	//Two-line plates
	//============================================================================

	//No character space between the text

	//Maximum 4 characters on top and bottom line - legal
	//Minimum 7mm gap between top and bottom lines - legal

	//Line gap should be 13mm and can be increased as long as padding
	//from border is 7mm and padding from plate edge is 11mm - legal

	//For 4 or less characters user has option to use one line (centered) or two lines

	//============================================================================

	plateData.name = "Motorcycle 7x5\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 178;
	plateData.dimensionMM.y = 127;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = false;
	plateData.isLegal = false;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Motorcycle 7x5 Inch"] = plateData;

	//============================================================================

	plateData.name = "Motorcycle 8x6\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 203;
	plateData.dimensionMM.y = 152;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = false;
	plateData.isLegal = false;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Motorcycle 8x6 Inch"] = plateData;

	//============================================================================

	plateData.name = "Motorcycle 9x7\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 235;
	plateData.dimensionMM.y = 178;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = false;
	plateData.isLegal = true;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Motorcycle 9x7 Inch"] = plateData;

	//============================================================================

	plateData.name = "Square 11x8\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 279;
	plateData.dimensionMM.y = 203;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Square 11x8 Inch"] = plateData;

	//============================================================================

	plateData.name = "USA Import 12x6\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 310;
	plateData.dimensionMM.y = 152;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = true;

	plateDataMap["USA Import 12x6 Inch"] = plateData;

	//============================================================================

	plateData.name = "Large Euro Square";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 310;
	plateData.dimensionMM.y = 203;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = true;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Large Euro Square"] = plateData;

	//============================================================================

	plateData.name = "Import 13x6.5\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 330;
	plateData.dimensionMM.y = 165;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Import 13x6.5 Inch"] = plateData;

	//============================================================================

	plateData.name = "Import 13x7\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 330;
	plateData.dimensionMM.y = 178;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = true;
	plateData.isCarFont = false;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = true;

	plateDataMap["Import 13x7 Inch"] = plateData;

	//============================================================================

	plateData.name = "Micro Plate 9x3\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 228;
	plateData.dimensionMM.y = 76;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = false;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Micro Plate 9x3 Inch"] = plateData;

	//============================================================================

	plateData.name = "Micro Plate 12x3\"";
	plateData.fontNameBSAU = "Arial_bold_4mm";
	plateData.fontNameDealerPostcode = "Arial_bold_7mm";

	plateData.dimensionMM.x = 310;
	plateData.dimensionMM.y = 76;
	plateData.dimensionNDC = NormalizePlate(plateData.dimensionMM);

	plateData.maxAllowedCharacters = 7;

	plateData.isCar = true;
	plateData.isLegal = false;
	plateData.isCarFont = true;
	plateData.isSideBadgeAllowed = false;
	plateData.isTwoLineRegistration = false;

	plateDataMap["Micro Plate 12x3 Inch"] = plateData;
}
//================================================================================================
void Plate::Print(const Plate& plate,
	PropertiesPanel::Properties& properties,
	const glm::vec2& maxDimension,
	Shader& shader,
	bool isPrinting)
{
	auto oldMaxDimension = Plate::maxDimension;
	Plate::maxDimension = maxDimension;

	auto plateData = plate.GetPlateData();
	plateData.dimensionNDC = glm::vec2(2.0f, 2.0f);

	//We need to load temp. printing fonts
	//The height of the text is multiplied by 1.37 because 
	//the resulting pixel size is not always as we expect
	const auto scale = 1.37f;

	//Text sizes are between 4mm and 10mm
	//There is a bug when loading a preset .nps 
	//file and then changing to Print/3D view mode after
	//Because the folder system is reset we have to put '../Assets' to get out of the Data folder
	Text::Load("Assets/Fonts/Arial_bold.ttf", 10.0f * scale, "Arial_bold_10mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 8.0f * scale, "Arial_bold_8mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 7.0f * scale, "Arial_bold_7mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 6.0f * scale, "Arial_bold_6mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 5.0f * scale, "Arial_bold_5mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 4.0f * scale, "Arial_bold_4mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 3.0f * scale, "Arial_bold_3mm_print");

	Registration::Load("Assets/Images/Fonts/2D_car", "Car_2D_print");
	Registration::Load("Assets/Images/Fonts/3D_car_under", "Car_3D_print");
	Registration::Load("Assets/Images/Fonts/4D_car_under", "Car_4D_print");

	Registration::Load("Assets/Images/Fonts/2D_motorcycle", "Bike_2D_print");
	Registration::Load("Assets/Images/Fonts/3D_motorcycle_under", "Bike_3D_print");
	Registration::Load("Assets/Images/Fonts/4D_motorcycle_under", "Bike_4D_print");

	plateData.fontNameBSAU = plate.GetPlateData().fontNameBSAU + "_print";

	Plate printPlate(properties);
	printPlate.SetPlateData("Custom", &plateData);

	//Why do we have a 'isPrinting' flag and pass a flag to 'SetProperties'?
	Plate::isPrinting = isPrinting;

	printPlate.SetProperties(true);
	printPlate.Render(shader);

	Plate::isPrinting = false;

	Plate::maxDimension = oldMaxDimension;

	//We need to unload the temp fonts again
	Text::Unload("Arial_bold_10mm_print");
	Text::Unload("Arial_bold_8mm_print");
	Text::Unload("Arial_bold_7mm_print");
	Text::Unload("Arial_bold_6mm_print");
	Text::Unload("Arial_bold_5mm_print");
	Text::Unload("Arial_bold_4mm_print");
	Text::Unload("Arial_bold_3mm_print");

	Registration::Unload("Bike_4D_print");
	Registration::Unload("Bike_3D_print");
	Registration::Unload("Bike_2D_print");

	Registration::Unload("Car_4D_print");
	Registration::Unload("Car_3D_print");
	Registration::Unload("Car_2D_print");
}
//================================================================================================
bool Plate::IsLegal() const
{
	return isLegal;
}
//================================================================================================
void Plate::IsLegal(bool flag)
{
	isLegal = flag;
}
//================================================================================================
Transform& Plate::GetTransform()
{
	return transform;
}
//================================================================================================
glm::vec2 Plate::GetMaxDimension()
{
	return maxDimension;
}
//================================================================================================
const Text* Plate::GetBSAUText() const
{
	return BSAUText.get();
}
//================================================================================================
const Text* Plate::GetDealerText() const
{
	return dealerText.get();
}
//================================================================================================
const Text* Plate::GetPostcodeText() const
{
	return postcodeText.get();
}
//================================================================================================
const PropertiesPanel::Properties& Plate::GetProperties() const
{
	return properties;
}
//================================================================================================
const Plate::PlateData& Plate::GetPlateData() const
{
	return plateData;
}
//================================================================================================
Plate::Plate(PropertiesPanel::Properties& properties) : properties(properties)
{
	plateData = plateDataMap["Standard Oblong"];

	border = std::make_unique<Border>(this);

	registration = std::make_unique<Registration>(this);
	registration->SetFont("Car_2D");
	registration->SetString("REG 123");

	//The height of the text is 8mm
	dealerText = std::make_unique<Text>(this, "Arial_bold_8mm");
	postcodeText = std::make_unique<Text>(this, "Arial_bold_8mm");
	BSAUText = std::make_unique<Text>(this, "Arial_bold_8mm");

	static auto totalPlateObjects = 0;

	const auto middleBufferSize = 30;
	const auto cornerBufferSize = 4 * (maxCornerPoints + 1);

	middle = std::make_unique<Buffer>(middleBufferSize, true);
	middle->LinkEBO();

	corners = std::make_unique<Buffer>(cornerBufferSize, false);

	totalPlateObjects++;
	FillBuffers();
}
//================================================================================================
void Plate::SetProperties(bool isPrinting)
{
	auto ToNDC = [](int value, float maxValue)
	{
		return 2.0f * (value / maxValue);
	};

	auto position = transform.GetPosition();

	//=================================================================================
	//Font
	//=================================================================================

	/*if (properties.isCarFont)
	{
		if (properties.is2DRegular)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_2D_print");
			}

			else
			{
				registration->SetFont("Car_2D");
			}
		}

		else if (properties.is3DGelResin)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_3D_print");
			}

			else
			{
				registration->SetFont("Car_3D");
			}
		}

		else if (properties.is4DLaserCut)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_4D_print");
			}

			else
			{
				registration->SetFont("Car_4D");
			}
		}
	}

	else
	{
		if (properties.is2DRegular)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_2D_print");
			}

			else
			{
				registration->SetFont("Bike_2D");
			}
		}

		else if (properties.is3DGelResin)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_3D_print");
			}

			else
			{
				registration->SetFont("Bike_3D");
			}
		}

		else if (properties.is4DLaserCut)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_4D_print");
			}

			else
			{
				registration->SetFont("Bike_4D");
			}
		}
	}*/

	//=================================================================================
	//Plate
	//=================================================================================

	properties.plateName = plateData.name;

	if (properties.plateWidth != plateData.dimensionMM.x ||
		properties.plateHeight != plateData.dimensionMM.y)
	{
		PlateData plateData = this->plateData;
		plateData.name = "Custom";

		plateData.dimensionMM.x = properties.plateWidth;
		plateData.dimensionMM.y = properties.plateHeight;

		plateData.dimensionNDC.x = 2.0f * (plateData.dimensionMM.x / maxDimension.x);
		plateData.dimensionNDC.y = 2.0f * (plateData.dimensionMM.y / maxDimension.y);

		SetPlateData("Custom", &plateData);
	}

	//=================================================================================
	//Registration
	//=================================================================================

	registration->SetString(properties.registrationText);

	position.x += ToNDC(properties.nudgedRegistration, maxDimension.x);
	position.y += ToNDC(properties.raisedRegistration, maxDimension.y);

	if (properties.isSideBadgeVisible)
	{
		position.x += ToNDC(this->sideBadgeMargin, maxDimension.x) * 0.5f;
	}

	registration->GetTransform().SetPosition(position);

	//=================================================================================
	//Dealer and postcode 
	//=================================================================================

	dealerText->SetString(properties.dealerText);
	postcodeText->SetString(properties.postcodeText);

	position = transform.GetPosition();

	if (properties.isDealerAbovePostcode)
	{
		if (properties.isSideBadgeVisible)
		{
			position.x += ToNDC(this->sideBadgeMargin, maxDimension.x) * 0.5f;
		}

		const auto dealerPostcodePaddingY = ToNDC(this->dealerPostcodePaddingY, maxDimension.y);
		const auto postcodeBottomPadding = ToNDC(this->postcodeBottomPadding, maxDimension.y);

		const auto postcodeWithPaddingHeight = (postcodeText->GetMaxHeight()
			+ dealerPostcodePaddingY
			+ postcodeBottomPadding);

		//Dealer
		position.x -= 0.5f * dealerText->GetMaxWidth() - ToNDC(properties.nudgedDealer, maxDimension.x);
		position.y -= 0.5f * plateData.dimensionNDC.y - postcodeWithPaddingHeight;
		position.y += ToNDC(properties.raisedDealer, maxDimension.y);

		dealerText->GetTransform().SetPosition(position);

		//Postcode
		position.x += ToNDC(properties.nudgedPostcode, maxDimension.x);
		position.y -= postcodeWithPaddingHeight - (postcodeBottomPadding);
		position.y += ToNDC(properties.raisedPostcode, maxDimension.y);

		postcodeText->GetTransform().SetPosition(position);
	}

	else
	{
		if (properties.isSideBadgeVisible)
		{
			position.x += ToNDC(this->sideBadgeMargin, maxDimension.x) * 0.5f;
		}

		const auto dealerBottomPadding = ToNDC(this->dealerBottomPadding, maxDimension.y);
		const auto dealerPostcodePaddingX = ToNDC(this->dealerPostcodePaddingX, maxDimension.x);

		const auto width = dealerText->GetMaxWidth()
			+ postcodeText->GetMaxWidth()
			+ dealerPostcodePaddingX;

		//Dealer
		position.x -= 0.5f * width - (ToNDC(properties.nudgedDealer, maxDimension.x));
		position.y -= 0.5f * plateData.dimensionNDC.y - dealerBottomPadding;
		position.y += ToNDC(properties.raisedDealer, maxDimension.y);

		dealerText->GetTransform().SetPosition(position);

		//Postcode
		position.x += dealerText->GetMaxWidth() + dealerPostcodePaddingX
			+ (ToNDC(properties.nudgedPostcode, maxDimension.x));
		position.y += ToNDC(properties.raisedPostcode, maxDimension.y);

		postcodeText->GetTransform().SetPosition(position);
	}

	//=================================================================================
	//BSAU 
	//=================================================================================

	BSAUText->SetString(properties.BSAUText);

	position = transform.GetPosition();

	//Text padding for BSAU text
	const auto BSAUPadding = ToNDC(this->textPadding, maxDimension.x);

	if (properties.isBSAUOnBorder)
	{
		const auto marginX = ToNDC(properties.marginSize, maxDimension.x);
		const auto marginY = ToNDC(properties.marginSize, maxDimension.y);

		const auto sideDimension = ToNDC((int)this->sideDimension.x, maxDimension.x);
		const auto width = sideDimension + marginX + BSAUText->GetMaxWidth() + BSAUPadding;

		position.x += 0.5f * plateData.dimensionNDC.x - width + ToNDC(properties.nudgedBSAU, maxDimension.x);
		position.y -= 0.5f * plateData.dimensionNDC.y - marginY;
		position.y += ToNDC(properties.raisedBSAU, maxDimension.y);

		BSAUText->GetTransform().SetPosition(position);
	}

	else
	{
		const auto size = ToNDC(properties.borderSize, maxDimension.y);
		const auto marginX = ToNDC(properties.marginSize, maxDimension.x);
		const auto marginY = ToNDC(properties.marginSize, maxDimension.y);

		const auto sideDimension = ToNDC((int)this->sideDimension.x, maxDimension.x);
		const auto width = sideDimension * plateData.dimensionNDC.x + marginX + BSAUText->GetMaxWidth();

		const auto height = marginY + size;

		position.x += 0.5f * plateData.dimensionNDC.x - width + ToNDC(properties.nudgedBSAU, maxDimension.x);
		position.y -= 0.5f * plateData.dimensionNDC.y - height;
		position.y += ToNDC(properties.raisedBSAU, maxDimension.y);

		BSAUText->GetTransform().SetPosition(position);
	}

	//=================================================================================
	//Legality checks
	//=================================================================================

	isLegal = properties.isBSAUVisible && properties.isDealerVisible;

	//If user has requested a side badge but this is not allowed
	//for this particular plate then the plate becomes illegal
	if (properties.isSideBadgeVisible && !plateData.isSideBadgeAllowed)
	{
		isLegal = false;
	}
}
//================================================================================================
void Plate::SetPlateData(const std::string& name, PlateData* plateData)
{
	if (name == "Custom")
	{
		this->plateData = *plateData;
	}

	else
	{
		this->plateData = plateDataMap[name];
	}

	FillBuffers();

	BSAUText->SetFont(this->plateData.fontNameBSAU);
	dealerText->SetFont(this->plateData.fontNameDealerPostcode);
	postcodeText->SetFont(this->plateData.fontNameDealerPostcode);

	properties.plateWidth = this->plateData.dimensionMM.x;
	properties.plateHeight = this->plateData.dimensionMM.y;
	properties.isTwoLineRegistration = this->plateData.isTwoLineRegistration;
}
//================================================================================================
void Plate::Render(Shader& shader)
{
	shader.SendData("isText", false);
	shader.SendData("isTextured", false);
	shader.SendData("model", transform.GetMatrix());

	//================================================================

	if (!isPrinting)
	{
		middle->LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		middle->LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

		middle->Render(Buffer::RenderMode::Triangles);

		corners->LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		corners->LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

		corners->Render(Buffer::RenderMode::TriangleFan);
	}

	//================================================================

	if (properties.isBorderVisible)
	{
		border->Render(shader);
	}

	/*bool isFontSelected = (m_plateState.is2DRegularOn
		|| m_plateState.is3DGelResinOn
		|| m_plateState.is4DLaserCutOn);*/

	if (customFontRegistration)
	{
		auto position = glm::vec3(0.0f);

		//We need to scale the font size up by 8 to 'pull it down' (WIP)
		position.x -= customFontRegistration->GetMaxWidth() * 0.5f;
		position.y -= customFontRegistration->GetMaxHeight() * 0.35f;

		customFontRegistration->GetTransform().SetPosition(position);
		customFontRegistration->SetString(registration->GetString());
		customFontRegistration->Render(shader);
	}

	else
	{
		registration->Render(shader);
	}

	if (properties.isBSAUVisible)
	{
		BSAUText->Render(shader);
	}

	if (properties.isDealerVisible)
	{
		dealerText->Render(shader);
		postcodeText->Render(shader);
	}
}
//================================================================================================
bool Plate::LoadCustomFont(FontToChange fontToChange, const std::string& filename, GLfloat fontSize)
{
	if (fontToChange == FontToChange::Registration)
	{
		//There could be an old version loaded already
		Text::Unload("CustomFontRegistration");

		//Load the new font and use the same tag since we only ever need one at a time
		Text::Load(filename, fontSize, "CustomFontRegistration");

		customFontRegistration.reset();
		customFontRegistration = std::make_unique<Text>(this, "CustomFontRegistration");
		customFontRegistration->SetString(registration->GetString());
	}

	else if (fontToChange == FontToChange::Dealer)
	{
		Text::Unload("CustomFontDealer");
		Text::Load(filename, fontSize, "CustomFontDealer");

		auto text = dealerText->GetString();
		
		dealerText.reset();
		dealerText = std::make_unique<Text>(this, "CustomFontDealer");
		dealerText->SetString(text);
	}

	else if (fontToChange == FontToChange::Postcode)
	{
		Text::Unload("CustomFontPostcode");
		Text::Load(filename, fontSize, "CustomFontPostcode");

		auto text = postcodeText->GetString();

		postcodeText.reset();
		postcodeText = std::make_unique<Text>(this, "CustomFontPostcode");
		postcodeText->SetString(text);
	}

	else if (fontToChange == FontToChange::BSAU)
	{
		Text::Unload("CustomFontBSAU");
		Text::Load(filename, fontSize, "CustomFontBSAU");

		auto text = BSAUText->GetString();

		BSAUText.reset();
		BSAUText = std::make_unique<Text>(this, "CustomFontBSAU");
		BSAUText->SetString(text);
	}

	return true;
}
//================================================================================================
void Plate::LoadDefaultFont(FontDialog::FontType fontType, FontDialog::FontStyle fontStyle)
{
	if (fontType.isCar)
	{
		if (fontStyle == FontDialog::FontStyle::Regular2D)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_2D_print");
			}

			else
			{
				registration->SetFont("Car_2D");
			}
		}

		else if (fontStyle == FontDialog::FontStyle::GelResin3D)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_3D_print");
			}

			else
			{
				registration->SetFont("Car_3D");
			}
		}

		else if (fontStyle == FontDialog::FontStyle::Lasercut4D)
		{
			if (isPrinting)
			{
				registration->SetFont("Car_4D_print");
			}

			else
			{
				registration->SetFont("Car_4D");
			}
		}
	}

	else
	{
		if (fontStyle == FontDialog::FontStyle::Regular2D)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_2D_print");
			}

			else
			{
				registration->SetFont("Bike_2D");
			}
		}

		else if (fontStyle == FontDialog::FontStyle::GelResin3D)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_3D_print");
			}

			else
			{
				registration->SetFont("Bike_3D");
			}
		}

		else if (fontStyle == FontDialog::FontStyle::Lasercut4D)
		{
			if (isPrinting)
			{
				registration->SetFont("Bike_4D_print");
			}

			else
			{
				registration->SetFont("Bike_4D");
			}
		}
	}

	//'Deactivate' the custom font so that it's a null object when 
	//we render and the normal registration object will render instead
	customFontRegistration.reset();
}
//================================================================================================
void Plate::FillBuffers()
{
	const auto sideDimension = 2.0f * (this->sideDimension / maxDimension);

	auto halfDimension = glm::vec2(plateData.dimensionNDC) * 0.5f;
	auto middleDimension = halfDimension - sideDimension;

	//main block 
	GLfloat vertices[] = { -middleDimension.x,  middleDimension.y, 0.0f,
							middleDimension.x,  middleDimension.y, 0.0f,
							middleDimension.x, -middleDimension.y, 0.0f,
						   -middleDimension.x, -middleDimension.y, 0.0f,

		//top block
		-middleDimension.x,  halfDimension.y, 0.0f,
		 middleDimension.x,  halfDimension.y, 0.0f,
		 middleDimension.x,  middleDimension.y, 0.0f,
		-middleDimension.x,  middleDimension.y, 0.0f,

		//bottom block
		-middleDimension.x, -middleDimension.y, 0.0f,
		 middleDimension.x, -middleDimension.y, 0.0f,
		 middleDimension.x, -halfDimension.y, 0.0f,
		-middleDimension.x, -halfDimension.y, 0.0f,

		//left block
		-halfDimension.x,    middleDimension.y, 0.0f,
		-middleDimension.x,  middleDimension.y, 0.0f,
		-middleDimension.x, -middleDimension.y, 0.0f,
		-halfDimension.x,   -middleDimension.y, 0.0f,

		//right block
		middleDimension.x,  middleDimension.y, 0.0f,
		halfDimension.x,    middleDimension.y, 0.0f,
		halfDimension.x,   -middleDimension.y, 0.0f,
		middleDimension.x, -middleDimension.y, 0.0f };

	//main block 
	GLfloat colors[] = { 1.0f, 0.8f, 0.2f, 1.0f,
						 1.0f, 0.8f, 0.2f, 1.0f,
						 1.0f, 0.8f, 0.2f, 1.0f,
						 1.0f, 0.8f, 0.2f, 1.0f,

		//top block
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,

		//bottom block
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,

		//left block
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,

		//right block
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f,
		1.0f, 0.8f, 0.2f, 1.0f };

	GLuint indices[] = { 0,  1,  3,  3,  1,  2,
						  4,  5,  7,  7,  5,  6,
						  8,  9, 11, 11,  9, 10,
						 12, 13, 15, 15, 13, 14,
						 16, 17, 19, 19, 17, 18 };

	middle->FillVBO(Buffer::VBO::VertexBuffer,
		vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	middle->FillVBO(Buffer::VBO::ColorBuffer,
		colors, sizeof(colors), Buffer::Fill::Ongoing);
	middle->FillEBO(indices, sizeof(indices), Buffer::Fill::Ongoing);

	//================================================================================

	std::vector<GLfloat> colorsCorners;
	std::vector<GLfloat> verticesCorners;

	for (GLuint i = 0; i < 4 * (maxCornerPoints + 1); i++)
	{
		colorsCorners.push_back(1.0f);
		colorsCorners.push_back(0.8f);
		colorsCorners.push_back(0.2f);
		colorsCorners.push_back(1.0f);
	}

	verticesCorners.push_back(middleDimension.x);
	verticesCorners.push_back(middleDimension.y);
	verticesCorners.push_back(0.0f);

	auto angle = 0.0f;
	auto angleIncrements = 90.0f / (maxCornerPoints - 1);

	for (GLuint i = 0; i < maxCornerPoints; i++)
	{
		verticesCorners.push_back(cos(glm::radians(angle)) * sideDimension.x + middleDimension.x);
		verticesCorners.push_back(sin(glm::radians(angle)) * sideDimension.y + middleDimension.y);

		verticesCorners.push_back(0.0f);
		angle += angleIncrements;
	}

	//================================================================================

	verticesCorners.push_back(-middleDimension.x);
	verticesCorners.push_back(middleDimension.y);
	verticesCorners.push_back(0.0f);

	angle = 90.0f;

	for (GLuint i = 0; i < maxCornerPoints; i++)
	{
		verticesCorners.push_back(cos(glm::radians(angle)) * sideDimension.x - middleDimension.x);
		verticesCorners.push_back(sin(glm::radians(angle)) * sideDimension.y + middleDimension.y);
		verticesCorners.push_back(0.0f);
		angle += angleIncrements;
	}

	//================================================================================

	verticesCorners.push_back(-middleDimension.x);
	verticesCorners.push_back(-middleDimension.y);
	verticesCorners.push_back(0.0f);

	angle = 180.0f;

	for (GLuint i = 0; i < maxCornerPoints; i++)
	{
		verticesCorners.push_back(cos(glm::radians(angle)) * sideDimension.x - middleDimension.x);
		verticesCorners.push_back(sin(glm::radians(angle)) * sideDimension.y - middleDimension.y);
		verticesCorners.push_back(0.0f);
		angle += angleIncrements;
	}

	//================================================================================

	verticesCorners.push_back(middleDimension.x);
	verticesCorners.push_back(-middleDimension.y);
	verticesCorners.push_back(0.0f);

	angle = 270.0f;

	for (GLuint i = 0; i < maxCornerPoints; i++)
	{
		verticesCorners.push_back(cos(glm::radians(angle)) * sideDimension.x + middleDimension.x);
		verticesCorners.push_back(sin(glm::radians(angle)) * sideDimension.y - middleDimension.y);
		verticesCorners.push_back(0.0f);
		angle += angleIncrements;
	}

	corners->FillVBO(Buffer::VBO::VertexBuffer,
		&verticesCorners[0], sizeof(GLfloat) * verticesCorners.size(), Buffer::Fill::Ongoing);
	corners->FillVBO(Buffer::VBO::ColorBuffer,
		&colorsCorners[0], sizeof(GLfloat) * colorsCorners.size(), Buffer::Fill::Ongoing);
}