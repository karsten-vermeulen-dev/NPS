#pragma once

#include <map>
#include <memory>
#include <string>
#include <glm.hpp>
#include "GLAD/gl.h"

#include "Border.h"
#include "Buffer.h"
#include "Properties.h"
#include "Registration.h"
#include "Shader.h"
#include "Text.h"
#include "Transform.h"
#include "FontDialog.h"

class Plate
{

public:

	//The spacing on either side of the dealer and BSAU text is 10mm
	const GLuint textPadding = 10;

	//The more corner points the rounder the corners
	const GLuint maxCornerPoints = 25;

	//The spacing required on the left side of the plate for side badges is 45mm
	const GLuint sideBadgeMargin = 45;

	//The amount of space for the side edges of the plate is 10mm
	const glm::vec2 sideDimension = glm::vec2(10.0f);

	//The width spacing between the dealer and postcode text is 2mm (if dealer next to postcode)
	const GLuint dealerPostcodePaddingX = 2;

	//The height spacing between the dealer and postcode text is 2mm (if dealer above postcode)
	const GLuint dealerPostcodePaddingY = 2;

	//The spacing between the dealer name and the bottom of the plate is 3mm
	const GLuint dealerBottomPadding = 3;

	//The spacing between the postcode name and the bottom of the plate is 3mm
	const GLuint postcodeBottomPadding = 3;

	//The spacing between the dealer name and the bottom of the plate is 7mm (if above the postcode)
	//const GLuint dealerBottomPaddingAbove = 7;

	enum class FontToChange
	{
		Registration,
		Dealer,
		Postcode,
		BSAU,
		DealerPostcodeBSAU
	};

	static void Initialize(const std::string& filename, GLfloat maxWidth);

	static void Print(const Plate& plate,
		Properties* properties,
		const glm::vec2& maxDimension,
		Shader& shader,
		bool isPrinting);

	Plate(const std::string& tag);

	bool IsLegal() const;
	void IsLegal(bool flag);

	Transform& GetTransform();

	static glm::vec2 GetMaxDimension();

	const glm::vec2& GetLegalDimensionNDC() const;

	const std::map<std::string, std::string>& GetPlateData() const;

	const Text* GetBSAUText() const;
	const Text* GetDealerText() const;
	const Text* GetPostcodeText() const;

	const Properties* GetProperties() const;

	//Temp. fix using 'isPrinting' parameter to set the 
	//printing state on/off to load correct-sized reg. fonts
	//Should we remove this temp. fix later?
	//Should we move all code from this function to the 'Render' function?
	void SetProperties(bool isPrinting = false);

	void SetPlateData(const std::string& tag);

	void SetUserInterfaces(Properties* properties, FontDialog* fontBox);

	void Render(Shader& shader);

	bool LoadCustomFont(FontToChange fontToChange, const std::string& filename, GLfloat fontSize);

	void LoadDefaultFont(FontDialog::FontType fontType, FontDialog::FontStyle fontStyle);

private:

	//The maximum width and height a number plate
	//can be within the given viewport (mm)
	static glm::vec2 maxDimension;

	//Flag to set when in printing mode so that when we call the Render()
	//function, we can disable the rendering of the yellow plate 
	static bool isPrinting;

	//The globally accessible data map containing information about all the plates
	static std::map<std::string, std::map<std::string, std::string>> plateDataMap;

	//We need our own local map of data pertaining to this 
	//particular plate ("Standard Oblong", "Micro Plate", etc)
	std::map<std::string, std::string> plateData;

	void FillBuffers();

	bool isLegal{ true };
	bool hasTwoLines{ false };

	std::unique_ptr<Buffer> middle;
	std::unique_ptr<Buffer> corners;
	std::unique_ptr<Border> border;

	std::unique_ptr<Text> BSAUText;
	std::unique_ptr<Text> dealerText;
	std::unique_ptr<Text> postcodeText;

	std::unique_ptr<Registration> registration;

	//Not fully working/tested yet
	std::unique_ptr<Text> customFontRegistration;

	Transform transform; //Is this really needed since the plate is always at 0,0?

	//We need a means of identifying THIS plate
	std::string tag;

	//Official legal dimensions (mm and NDC) of the current plate (not actual dimension!)
	//We require these in various places so let's make them members 
	glm::uvec2 legalDimension;
	glm::vec2 legalDimensionNDC;

	//This is hooked up to the properties panel UI
	//PropertiesPanel::Properties& properties;
	Properties* properties;

	FontDialog* fontBox;

};