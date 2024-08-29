#pragma once

#include <map>
#include <set>
#include <vector>
#include "UserInterface.h"

//There is a debate whether to have this Font dialog with all of its settings
//or to add these settings into the Properties panel. Having this dialog box
//means that the user can easily control all font properties under one roof
//instead of scattered font settings in the Properties panel. This avoids 
//having multiple custom font combo boxes and condenses the Properties panel

class FontSettings : public UserInterface
{

public:

	//Enums work best with the index values of combo boxes
	enum class FontStyle
	{
		Regular2D, 
		GelResin3D, 
		Lasercut4D 
	};

	struct ButtonState
	{
		bool ok{ false };
		bool cancel{ false };
	};

	bool isRegistration{ true };
	bool isDealer{ false };
	bool isPostcode{ false };
	bool isBSAU{ false };
	
	bool isCar{ true };
	bool isMotorCycle{ false };
	bool isCustom{ false };

	//We need a font string for all font types - reg, dealer, postcode, bsau
	//This string will be the filename and path of the custom font file (.ttf/.otf) 
	std::string font;

	FontStyle fontStyle{ FontStyle::Regular2D };
	
	ButtonState buttonState;

	FontSettings(const std::string& filename);

	virtual void Show() override;
	virtual void Reset() override;

private:

	//We store all font names and their respective file locations
	//The names are used by the combo box as the font index list
	std::map<std::string, std::string> fontNames;

	int comboBoxFontID{ 0 };
	int comboBoxFontStyleID{ 0 };
};