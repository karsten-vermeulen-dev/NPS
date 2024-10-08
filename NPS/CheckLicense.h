#pragma once
#include "UserInterface.h"

class CheckLicense : public UserInterface
{

public:

	struct ButtonState
	{
		bool okay{ false };
	};

	const ButtonState& GetButtonState();

	virtual void Show() override;
	virtual void Reset() override {}

private:

	ButtonState buttonState;

};