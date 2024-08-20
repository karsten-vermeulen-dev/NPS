#pragma once

#include <map>
#include <memory>
#include <string>
#include <glm.hpp>
#include "GLAD/gl.h"

#include "Border.h"
#include "Buffer.h"
#include "PropertiesPanel.h"
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

	struct PlateData
	{
		std::string name;                        //Official legal name of plate
		std::string fontNameBSAU;                //Name and size of font required for BSAU text           
		std::string fontNameDealerPostcode;      //Name and size of font required for Dealer/Postcode text

		glm::ivec2 dimensionMM{ 0 };             //Legal dimension of plate
		glm::vec2 dimensionNDC{ 0.0f };          //Converted NDC dimension for use in application (not saved to file)

		GLuint minAllowedGapMM{ 0 };             //The spacing in mm between lines in two-line plates
		GLuint maxAllowedSpace{ 0 };             //Maximum space legally permitted between characters
		GLuint maxAllowedCharacters{ 0 };        //Maximum characters legally permitted in registration

		bool isCar{ false };                     //Flag stating whether this plate is for a car or motorcycle
		bool isLegal{ false };                   //Flag stating whether this plate is legal at all (The 'islegal' flag within Plate class is a dynamic variable that can change based on properties set)
		bool isCarFont{ false };                 //Flag stating whether this plate owns a car font (Generally speaking car plates own car fonts)
		bool isSpaceRequired{ false };           //Flag stating whether a space between the characters is legally required
		bool isSideBadgeAllowed{ false };        //Flag stating whether a side badge is legally permitted for this plate
		bool isTwoLineRegistration{ false };     //Flag stating whether this plate is a one-liner or two-line plate (Generally speaking car plates are one-liners and motorcycle plates are two-line plates)
	};

	static void Initialize(GLfloat maxWidth);
	
	static void Print(const Plate& plate, 
		PropertiesPanel::Properties& properties, 
		const glm::vec2& maxDimension, 
		Shader& shader, 
		bool isPrinting);

	Plate(PropertiesPanel::Properties& properties);

	bool IsLegal() const;
	void IsLegal(bool flag);

	Transform& GetTransform();

	static glm::vec2 GetMaxDimension();

	const Text* GetBSAUText() const;
	const Text* GetDealerText() const;
	const Text* GetPostcodeText() const;

	const PlateData& GetPlateData() const;
	const PropertiesPanel::Properties& GetProperties() const;

	//Temp. fix using 'isPrinting' parameter to set the 
	//printing state on/off to load correct-sized reg. fonts
	//Should we remove this temp. fix later?
	//Should we move all code from this function to the 'Render' function?
	void SetProperties(bool isPrinting = false);

	void SetPlateData(const std::string& name, PlateData* plateData = nullptr);

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
	
	static std::map<std::string, PlateData> plateDataMap;

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

	Transform transform;
	PlateData plateData;

	//This is hooked up to the properties panel UI
	PropertiesPanel::Properties& properties;

	//Why don't we hook up the Font settings the same way?
	
	//Why don't we hook up the Feedback panel the same way?

};