#pragma once

#include "UserInterface.h"

class ModePanel : public UserInterface
{

public:

	struct Mode
	{
		bool isPrint{ false };
		bool isDesign{ true };
		bool is3DView{ false };
	};

	const Mode& GetMode();

	virtual void Show() override;
	virtual void Reset() override {}

private:

	Mode mode;

};