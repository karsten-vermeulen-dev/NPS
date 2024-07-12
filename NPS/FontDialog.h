#pragma once

#include <map>
#include <set>
#include <vector>
#include "UserInterface.h"

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

	FontDialog();

	const std::string& GetFont() const;

	virtual void Show() override;

private:

	FontType fontType;
	FontStyle fontStyle;
	ButtonState buttonState;
	FontToChange fontToChange;

	std::string font;
	std::map<std::string, std::string> fontNames;

};