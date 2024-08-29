#include <algorithm>
#include <vector>
#include "Plate.h"
#include "Screen.h"
#include "Utility.h"

bool Plate::isPrinting{ false };
glm::vec2 Plate::maxDimension{ 0.0f };
std::map<std::string, std::map<std::string, std::string>> Plate::plateDataMap;
//================================================================================================
void Plate::Initialize(GLfloat maxWidth)
{
	auto viewport = Screen::Instance()->GetViewport();
	maxDimension.x = maxWidth;
	maxDimension.y = maxDimension.x / (viewport.w / static_cast<float>(viewport.h));
}
//================================================================================================
void Plate::Print(const Plate& plate,
	Properties* properties,
	const glm::vec2& maxDimension,
	Shader& shader,
	bool isPrinting)
{
	auto oldMaxDimension = Plate::maxDimension;
	Plate::maxDimension = maxDimension;

	//We need to load temp. printing fonts
	//The height of the text is multiplied by 1.37 because 
	//the resulting pixel size is not always as we expect
	const auto scale = 1.37f;

	//Text sizes are between 4mm and 10mm
	//There is a bug when loading a preset .nps 
	//file and then changing to Print/3D view mode after
	//Because the folder system is reset we have to put '../Assets' to get out of the Data folder
	Text::Load("Assets/Fonts/Arial_bold.ttf", 10.0f * scale, maxDimension, "Arial_bold_10mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 8.0f * scale, maxDimension, "Arial_bold_8mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 7.0f * scale, maxDimension, "Arial_bold_7mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 6.0f * scale, maxDimension, "Arial_bold_6mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 5.0f * scale, maxDimension, "Arial_bold_5mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 4.0f * scale, maxDimension, "Arial_bold_4mm_print");
	Text::Load("Assets/Fonts/Arial_bold.ttf", 3.0f * scale, maxDimension, "Arial_bold_3mm_print");

	Registration::Load("Assets/Images/Fonts/2D_car", maxDimension, "Car_2D_print");
	Registration::Load("Assets/Images/Fonts/3D_car_under", maxDimension, "Car_3D_print");
	Registration::Load("Assets/Images/Fonts/4D_car_under", maxDimension, "Car_4D_print");

	Registration::Load("Assets/Images/Fonts/2D_motorcycle", maxDimension, "Bike_2D_print");
	Registration::Load("Assets/Images/Fonts/3D_motorcycle_under", maxDimension, "Bike_3D_print");
	Registration::Load("Assets/Images/Fonts/4D_motorcycle_under", maxDimension, "Bike_4D_print");

	//plateData.fontNameBSAU = plate.GetPlateData().fontNameBSAU + "_print";

	Plate printPlate(plate.tag);
	printPlate.SetPlateData("Custom");

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
const glm::vec2& Plate::GetLegalDimensionNDC() const
{
	return legalDimensionNDC;
}
//================================================================================================
const std::map<std::string, std::string>& Plate::GetPlateData() const
{
	return plateData;
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
const Properties* Plate::GetProperties() const
{
	return properties;
}
//================================================================================================
Plate::Plate(const std::string& tag, const std::string& filename)
{
	static bool isDataFileLoaded = false;

	if (!isDataFileLoaded && !filename.empty())
	{
		if (!Utility::LoadDataFile(filename, plateDataMap))
		{
			//assert if failed to load?
		}

		isDataFileLoaded = true;
	}

	//default plate name when creating the first plate
	plateData = plateDataMap[tag];

	this->tag = tag;

	std::vector<std::string> subStrings;
	Utility::ParseString(plateData["Dimension"], subStrings, ',');

	legalDimension.x = stoi(subStrings[0]);
	legalDimension.y = stoi(subStrings[1]);

	legalDimensionNDC.x = Utility::ConvertToNDC(legalDimension.x, maxDimension.x);
	legalDimensionNDC.y = Utility::ConvertToNDC(legalDimension.y, maxDimension.y);

	//properties->plateWidth = legalDimension.x;
	//properties->plateHeight = legalDimension.y;

	border = std::make_unique<Border>(this);

	registration = std::make_unique<Registration>(this);
	registration->SetFont("Car_2D");
	registration->SetString("REG 123");

	//The height of the text is 8mm
	dealerText = std::make_unique<Text>(this, plateData["FontNameDealerPostcode"]);
	postcodeText = std::make_unique<Text>(this, plateData["FontNameDealerPostcode"]);
	BSAUText = std::make_unique<Text>(this, plateData["FontNameBSAU"]);

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

	auto dimensionNDC = glm::vec2(0.0f);
	dimensionNDC.x = Utility::ConvertToNDC(properties->plateWidth, maxDimension.x);
	dimensionNDC.y = Utility::ConvertToNDC(properties->plateHeight, maxDimension.y);
	
	properties->plateName = tag;

	//If the dimension of the plate deviates from its 
	//legal dimension then it becomes a 'Custom' plate
	if (properties->plateWidth != legalDimension.x || 
		properties->plateHeight != legalDimension.y)
	{
		tag = "Custom"; 

		//Rebuild the plate with new dimensions
		FillBuffers();
	}

	//=================================================================================
	//Registration
	//=================================================================================

	registration->SetString(properties->registrationText);

	position.x += Utility::ConvertToNDC(properties->nudgedRegistration, maxDimension.x);
	position.y += Utility::ConvertToNDC(properties->raisedRegistration, maxDimension.y);

	if (properties->isSideBadgeVisible)
	{
		position.x += 0.5f * Utility::ConvertToNDC(this->sideBadgeMargin, maxDimension.x);
	}

	registration->GetTransform().SetPosition(position);

	//=================================================================================
	//Dealer and postcode 
	//=================================================================================

	dealerText->SetString(properties->dealerText);
	postcodeText->SetString(properties->postcodeText);

	position = transform.GetPosition();

	if (properties->isDealerAbovePostcode)
	{
		if (properties->isSideBadgeVisible)
		{
			position.x += 0.5f * Utility::ConvertToNDC(this->sideBadgeMargin, maxDimension.x);
		}

		const auto dealerPostcodePaddingY = Utility::ConvertToNDC(this->dealerPostcodePaddingY, maxDimension.y);
		const auto postcodeBottomPadding = Utility::ConvertToNDC(this->postcodeBottomPadding, maxDimension.y);

		const auto postcodeWithPaddingHeight = (postcodeText->GetMaxHeight()
			+ dealerPostcodePaddingY
			+ postcodeBottomPadding);

		//Dealer
		position.x -= 0.5f * dealerText->GetMaxWidth() - Utility::ConvertToNDC(properties->nudgedDealer, maxDimension.x);
		position.y -= 0.5f * dimensionNDC.y - postcodeWithPaddingHeight;
		position.y += Utility::ConvertToNDC(properties->raisedDealer, maxDimension.y);

		dealerText->GetTransform().SetPosition(position);

		//Postcode
		position.x += Utility::ConvertToNDC(properties->nudgedPostcode, maxDimension.x);
		position.y -= postcodeWithPaddingHeight - (postcodeBottomPadding);
		position.y += Utility::ConvertToNDC(properties->raisedPostcode, maxDimension.y);

		postcodeText->GetTransform().SetPosition(position);
	}

	else
	{
		if (properties->isSideBadgeVisible)
		{
			position.x += 0.5f * Utility::ConvertToNDC(this->sideBadgeMargin, maxDimension.x);
		}

		const auto dealerBottomPadding = Utility::ConvertToNDC(this->dealerBottomPadding, maxDimension.y);
		const auto dealerPostcodePaddingX = Utility::ConvertToNDC(this->dealerPostcodePaddingX, maxDimension.x);

		const auto width = dealerText->GetMaxWidth()
			+ postcodeText->GetMaxWidth()
			+ dealerPostcodePaddingX;

		//Dealer
		position.x -= 0.5f * width - Utility::ConvertToNDC(properties->nudgedDealer, maxDimension.x);
		position.y -= 0.5f * dimensionNDC.y - dealerBottomPadding;
		position.y += Utility::ConvertToNDC(properties->raisedDealer, maxDimension.y);

		dealerText->GetTransform().SetPosition(position);

		//Postcode
		position.x += dealerText->GetMaxWidth() + dealerPostcodePaddingX
			+ Utility::ConvertToNDC(properties->nudgedPostcode, maxDimension.x);
		position.y += Utility::ConvertToNDC(properties->raisedPostcode, maxDimension.y);

		postcodeText->GetTransform().SetPosition(position);
	}

	//=================================================================================
	//BSAU 
	//=================================================================================

	BSAUText->SetString(properties->BSAUText);

	position = transform.GetPosition();

	//Text padding for BSAU text
	const auto BSAUPadding = Utility::ConvertToNDC(this->textPadding, maxDimension.x);

	if (properties->isBSAUOnBorder)
	{
		const auto marginX = Utility::ConvertToNDC(properties->marginSize, maxDimension.x);
		const auto marginY = Utility::ConvertToNDC(properties->marginSize, maxDimension.y);

		const auto sideDimension = Utility::ConvertToNDC((int)this->sideDimension.x, maxDimension.x);
		const auto width = sideDimension + marginX + BSAUText->GetMaxWidth() + BSAUPadding;

		position.x += 0.5f * dimensionNDC.x - width + Utility::ConvertToNDC(properties->nudgedBSAU, maxDimension.x);
		position.y -= 0.5f * dimensionNDC.y - marginY;
		position.y += Utility::ConvertToNDC(properties->raisedBSAU, maxDimension.y);

		BSAUText->GetTransform().SetPosition(position);
	}

	else
	{
		const auto size = Utility::ConvertToNDC(properties->borderSize, maxDimension.y);
		const auto marginX = Utility::ConvertToNDC(properties->marginSize, maxDimension.x);
		const auto marginY = Utility::ConvertToNDC(properties->marginSize, maxDimension.y);

		const auto sideDimension = Utility::ConvertToNDC((int)this->sideDimension.x, maxDimension.x);
		const auto width = sideDimension * legalDimensionNDC.x + marginX + BSAUText->GetMaxWidth();

		const auto height = marginY + size;

		position.x += 0.5f * dimensionNDC.x - width + Utility::ConvertToNDC(properties->nudgedBSAU, maxDimension.x);
		position.y -= 0.5f * dimensionNDC.y - height;
		position.y += Utility::ConvertToNDC(properties->raisedBSAU, maxDimension.y);

		BSAUText->GetTransform().SetPosition(position);
	}

	//=================================================================================
	//Legality checks
	//=================================================================================

	isLegal = properties->isBSAUVisible && properties->isDealerVisible;

	//If user has requested a side badge but this is not allowed
	//for this particular plate then the plate becomes illegal
	//if (properties->isSideBadgeVisible && !plateData.isSideBadgeAllowed)
	if (properties->isSideBadgeVisible && !stoi(plateData["IsSideBadgeAllowed"]))
	{
		isLegal = false;
	}
}
//================================================================================================
void Plate::SetPlateData(const std::string& tag)
{
	this->tag = tag;
	plateData = plateDataMap[tag];

	std::vector<std::string> subStrings;
	Utility::ParseString(plateData["Dimension"], subStrings, ',');

	legalDimension.x = stoi(subStrings[0]);
	legalDimension.y = stoi(subStrings[1]);

	legalDimensionNDC.x = Utility::ConvertToNDC(legalDimension.x, maxDimension.x);
	legalDimensionNDC.y = Utility::ConvertToNDC(legalDimension.y, maxDimension.y);

	BSAUText->SetFont(plateData["FontNameBSAU"]);
	dealerText->SetFont(plateData["FontNameDealerPostcode"]);
	postcodeText->SetFont(plateData["FontNameDealerPostcode"]);

	properties->plateWidth = legalDimension.x;
	properties->plateHeight = legalDimension.y;

	properties->isTwoLineRegistration = stoi(plateData["IsTwoLineRegistration"]);

	//Rebuild the plate
	FillBuffers();
}
//================================================================================================
void Plate::SetUserInterfaces(Properties* properties, FontDialog* fontBox)
{
	this->properties = properties;
	this->fontBox = fontBox;
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

	if (properties->isBorderVisible)
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

	if (properties->isBSAUVisible)
	{
		BSAUText->Render(shader);
	}

	if (properties->isDealerVisible)
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
		Text::Load(filename, fontSize, maxDimension, "CustomFontRegistration");

		customFontRegistration.reset();
		customFontRegistration = std::make_unique<Text>(this, "CustomFontRegistration");
		customFontRegistration->SetString(registration->GetString());
	}

	else if (fontToChange == FontToChange::Dealer)
	{
		Text::Unload("CustomFontDealer");
		Text::Load(filename, fontSize, maxDimension, "CustomFontDealer");

		auto text = dealerText->GetString();
		
		dealerText.reset();
		dealerText = std::make_unique<Text>(this, "CustomFontDealer");
		dealerText->SetString(text);
	}

	else if (fontToChange == FontToChange::Postcode)
	{
		Text::Unload("CustomFontPostcode");
		Text::Load(filename, fontSize, maxDimension, "CustomFontPostcode");

		auto text = postcodeText->GetString();

		postcodeText.reset();
		postcodeText = std::make_unique<Text>(this, "CustomFontPostcode");
		postcodeText->SetString(text);
	}

	else if (fontToChange == FontToChange::BSAU)
	{
		Text::Unload("CustomFontBSAU");
		Text::Load(filename, fontSize, maxDimension, "CustomFontBSAU");

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

	auto dimensionNDC = glm::vec2(0.0f);

	//If the properties pointer is valid it means the UI 
	//has been hooked up, then use the regular dimensions
	if (properties)
	{
		dimensionNDC.x = Utility::ConvertToNDC(properties->plateWidth, maxDimension.x);
		dimensionNDC.y = Utility::ConvertToNDC(properties->plateHeight, maxDimension.y);
	}

	//Otherwise it means the UI has not been hooked up and this is 
	//the first run of FillBuffers, so use the legalDimension instead
	else
	{
		dimensionNDC = legalDimensionNDC;
	}

	auto halfDimension = 0.5f * dimensionNDC;
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