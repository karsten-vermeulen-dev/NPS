#pragma once
#include "UserInterface.h"

class LicenseDialog : public UserInterface
{

public:

	struct ButtonState
	{
		bool okay{ false };
	};

	const ButtonState& GetButtonState();

	virtual void Show() override;

private:

	ButtonState buttonState;

};