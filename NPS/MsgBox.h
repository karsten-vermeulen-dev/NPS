#pragma once
#include "UserInterface.h"

//Would love to call it 'MessageBox', but Windows 
//has one already which causes compiler conflicts

class MsgBox : public UserInterface
{

public:

	enum class ButtonType
	{
		Okay,
		YesNo,
		OkCancel
	};

	struct ButtonState
	{
		bool yes{ false };
		bool no{ false };
		bool ok{ false };
		bool cancel{ false };
	};

	const ButtonState& GetButtonState();

	void SetTitle(const std::string& title);
	void SetMessage(const std::string& message);
	void SetButtonType(ButtonType buttonType);

	virtual void Show() override;
	virtual void Reset() override {}

private:

	ButtonType buttonType;
	ButtonState buttonState;

	std::string title;
	std::string message;

};