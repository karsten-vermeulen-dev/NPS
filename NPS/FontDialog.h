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

class FontDialog : public UserInterface
{

public:

	struct ButtonState
	{
		bool ok{ false };
		bool cancel{ false };
	};

	struct FontToChange
	{
		bool isRegistration{ true };
		bool isDealer{ false };
		bool isPostcode{ false };
		bool isBSAU{ false };
	};

	struct FontType
	{
		bool isCar{ true };
		bool isMotorCycle{ false };
		bool isCustom{ false };
	};

	struct FontStyle
	{
		bool is2DRegular{ true };
		bool is3DGelResin{ false };
		bool is4DLaserCut{ false };
	};

	FontDialog(const std::string& filename);

	const std::string& GetFont() const;

	virtual void Show() override;

	//Make these public for easy access and ability to change when required
	//This public access also makes code more readable in main codebase
	FontType fontType;
	FontStyle fontStyle;
	ButtonState buttonState;
	FontToChange fontToChange;

private:

	//We need a font string for all font types - reg, dealer, postcode, bsau
	//This string will be the filename and path of the custom font file (.ttf/.otf) 
	std::string font;

	//We store all font names and their respective file locations
	//The names are used by the combo box as the font index list
	std::map<std::string, std::string> fontNames;

};