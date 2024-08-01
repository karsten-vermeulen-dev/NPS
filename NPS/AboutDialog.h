#pragma once
#include "UserInterface.h"

class AboutDialog : public UserInterface
{

public:

	virtual void Show() override;
	virtual void Reset() override {}

private:


};