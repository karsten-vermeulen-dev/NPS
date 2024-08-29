#pragma once

#include <vector>
#include "UserInterface.h"

class CustomerInfo : public UserInterface
{

public:

	struct Information
	{
		std::string fullName;
		std::string companyName;
		std::string emailAddress;
		std::string siteCode;
		std::string licenceKey;
		std::string expiryDate;
	};

	CustomerInfo();
	
	virtual void Show() override;
	virtual void Reset() override {}

private:

	std::vector<Information> customers;

};