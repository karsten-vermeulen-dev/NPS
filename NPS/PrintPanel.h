#pragma once

#include "UserInterface.h"

class PrintPanel : public UserInterface
{

public:

	struct ButtonState
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
	};

	const Metrics& GetMetrics() const;
	const ButtonState& GetButtonState();

	virtual void Show() override;

private:

	Metrics metrics;
	ButtonState buttonState;

};