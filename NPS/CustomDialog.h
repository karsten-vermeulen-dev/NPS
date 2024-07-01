#pragma once
#include "UserInterface.h"

class CustomDialog : public UserInterface
{

public:

	struct ButtonState
	{
		bool ok{ false };
		bool cancel{ false };
	};

	struct CustomStyle
	{
		bool isCar{ true };
		bool hasCarFont{ true };
		bool hasSideBadge{ false };
		bool isTwoLinePlate{ false };
		glm::ivec2 dimension{ 0 };
	};

	const CustomStyle& GetCustomStyle();

	virtual void Show() override;

private:

	ButtonState buttonState;
	CustomStyle customStyle;

};