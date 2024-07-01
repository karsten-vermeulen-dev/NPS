#pragma once
#include "UserInterface.h"

class ActivateDialog : public UserInterface
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

private:

	ButtonState buttonState;

};