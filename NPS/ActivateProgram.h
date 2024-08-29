#pragma once
#include "UserInterface.h"

class ActivateProgram : public UserInterface
{

public:

	struct ButtonState
	{
		bool activate{ false };
		bool cancel{ false };
		bool help{ false };
	};

	const ButtonState& GetButtonState();

	virtual void Show() override;
	virtual void Reset() override {}

private:

	ButtonState buttonState;

};