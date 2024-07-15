#pragma once

#include <map>
#include <set>
#include <vector>
#include "UserInterface.h"

//There is a debate whether to have this Font dialog with all of its settings
//or to add these settings into the Properties panel. Having this dialog box
//means that the user can easily control all font properties under one roof
//instead of scattered font settings in the Properties panel. This avoids 
//multiple custom font combo boxes for one and condenses the Properties panel

class FontDialog : public UserInterface
{

public:

	struct ButtonState
	{
		bool ok{ false };
		bool cancel{ false };
		bool apply{ false };
	};

	struct FontToChange
	{
		bool isRegistration{ true };
		bool isDealer{ false };
		bool isPostcode{ false };
		bool isBSAU{ false };
		bool isDealerPostcodeBSAU{ false };
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

	std::string font;
	std::map<std::string, std::string> fontNames;

};