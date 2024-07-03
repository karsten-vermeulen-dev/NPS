#pragma once

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
	};

	FontDialog();
	virtual void Show() override;

private:

	ButtonState buttonState;
	
	std::vector<std::pair<std::string, std::string>> fontNames;

};