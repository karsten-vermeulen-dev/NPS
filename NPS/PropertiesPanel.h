#pragma once

#include "UserInterface.h"

class PropertiesPanel : public UserInterface
{

public:

	//Use these when in 'Print' mode
	/*struct ButtonState
	{
		bool print{ false };
	};

	struct Metrics
	{
		int topMargin{ 0 };
		int leftMargin{ 0 };
		int plateWidth{ 0 };
		int plateHeight{ 0 };
		int wideCharacterWidth{ 0 };
		int wideCharacterHeight{ 0 };
		int narrowCharacterWidth{ 0 };
	};*/

	//const Metrics& GetMetrics() const;
	//const ButtonState& GetButtonState();

	struct ButtonState
	{
		bool design{ false };
		bool printPreview{ false };
		bool view3D{ false };
	};

	struct Properties
	{
		std::string plateName;

		int plateWidth{ 0 };
		int plateHeight{ 0 };

		bool isTwoLineRegistration{ false };
		
		int raisedRegistration{ 0 };
		int nudgedRegistration{ 0 };
		int raisedTwoLineSpace{ 0 };

		std::string registrationText{ "REG 123" };

		int borderSize{ 2 };
		int marginSize{ 2 };

		bool isBorderVisible{ true };
		bool isSideBadgeVisible{ false };

		int raisedDealer{ 0 };
		int nudgedDealer{ 0 };
		int raisedPostcode{ 0 };
		int nudgedPostcode{ 0 };

		bool isDealerVisible{ true };
		bool isDealerAbovePostcode{ false };

		std::string postcodeText{ "B6 7SJ" };
		std::string dealerText{ "Plate factory" };

		int raisedBSAU{ 0 };
		int nudgedBSAU{ 0 };

		bool isBSAUVisible{ true };
		bool isBSAUOnBorder{ true };

		std::string BSAUText{ "NPM BS AU 145e" };
	};

	Properties& GetProperties();

	virtual void Show() override;
	virtual void Reset() override;

	//Make private later with a getter?
	ButtonState buttonState;

private:

	Properties properties;

	//Use these when in 'Print' mode
	//Metrics metrics;
	//ButtonState buttonState;
};