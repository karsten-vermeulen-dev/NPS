#pragma once

#include "UserInterface.h"

class Properties : public UserInterface
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

	//We set the initial values here so that the Properties 
	//panel starts up with the correct startup information
	std::string plateName{ "Standard Oblong" };

	int plateWidth{ 520 };
	int plateHeight{ 111 };

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

	ButtonState buttonState;
	
	virtual void Show() override;
	virtual void Reset() override;

private:

	//Use these when in 'Print' mode
	//Metrics metrics;
};