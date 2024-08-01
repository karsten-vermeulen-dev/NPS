#pragma once

#include "UserInterface.h"

class MainMenu : public UserInterface
{

public:

	struct MenuItems
	{
		bool isNewSelected{ false };
		bool isLoadSelected{ false };
		bool isSaveSelected{ false };
		bool isSaveAsSelected{ false };
		bool isExitSelected{ false };
		
		bool isStandardOblongSelected{ false };
		bool isShort13InchSelected{ false };
		bool isShort16InchSelected{ false };
		bool isShorter17InchSelected{ false };
		bool isShorter18InchSelected{ false };
		bool isOversizeOblongSelected{ false };
		bool isShapedPlateSelected{ false };
		
		bool isMotorcycle7x5InchSelected{ false };
		bool isMotorcycle8x6InchSelected{ false };
		bool isMotorcycle9x7InchSelected{ false };
		bool isSquare11x8InchSelected{ false };
		bool isUSAImport12x6InchSelected{ false };
		bool isLargeEuroSquare12x8InchSelected{ false };
		bool isImport13x6Point5InchSelected{ false };
		bool isImport13x7InchSelected{ false };
		
		bool isMicroPlate9x3InchSelected{ false };
		bool isMicroPlate12x3InchSelected{ false };
		bool isFontSettingsSelected{ false };
		
		bool isTutorialSelected{ false };
		bool isAboutSelected{ false };
		bool isActivateProgramSelected{ false };
		bool isCustomerInformationSelected{ false };
		bool isCheckLicenseSelected{ false };
	};

	void IsHost(bool flag);
	void IsActivated(bool flag);

	const MenuItems& GetMenuItems() const;

	virtual void Show() override;
	virtual void Reset() override {}

private:

	bool isHost{ true };
	bool isActivated{ false };

	MenuItems menuItems;

};