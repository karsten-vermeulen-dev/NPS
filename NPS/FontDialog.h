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
	};

	FontDialog();

	const std::string& GetChosenFont() const;

	virtual void Show() override;

private:

	ButtonState buttonState;
	
	std::string chosenFont;

	std::map<std::string, std::string> fontNames;

};