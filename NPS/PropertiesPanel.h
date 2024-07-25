#pragma once

#include "UserInterface.h"

class PropertiesPanel : public UserInterface
{

public:

	struct Properties
	{
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

private:

	Properties properties;

};