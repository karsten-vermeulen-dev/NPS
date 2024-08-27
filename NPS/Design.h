#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <glm.hpp>
#include <SDL.h>

#include "ActivateDialog.h"
#include "FontDialog.h"
#include "PrintPanel.h"
#include "CustomerDialog.h"
//#include "FeedbackPanel.h"
#include "Image.h"
#include "LicenseDialog.h"
#include "MainMenu.h"
#include "MessageDialog.h"
#include "ModePanel.h"
#include "Plate.h"
#include "PropertiesPanel.h"
#include "Screen.h"
#include "Shader.h"
#include "State.h"
#include "Text.h"

class Design : public State
{

public:

	virtual ~Design() {}
	virtual bool OnEnter();
	virtual State* Update();
	virtual bool Render();
	virtual void OnExit();

private:

	//void View3D();
	void PrintPlate();
	void SaveToFile(const std::string& filename, int DPI, bool hasAlphaChannel, bool isPrinting);

	void ResetView();

	State* newState{ nullptr };
	bool isStateComplete{ false };

	bool isCustomFontRequired{ false };

	bool isHost{ true };
	bool isActivated{ true };

	std::unique_ptr<Plate> plate;

	std::unique_ptr<Image> paper;
	std::unique_ptr<Image> backdrop;
	std::unique_ptr<Image> plateToPrint;

	std::unique_ptr<Shader> mainShader;
	std::unique_ptr<Shader> lightShader;

	std::unique_ptr<MainMenu> mainMenu;

	std::unique_ptr<FontDialog> fontDialog;
	
	//make this a container of dialogs - easier to render/show using a loop
	
	std::unique_ptr<MessageDialog> messageDialog;
	std::unique_ptr<ActivateDialog> activateDialog;
	std::unique_ptr<CustomerDialog> customerDialog;
	std::unique_ptr<LicenseDialog> licenseDialog;

	std::unique_ptr<ModePanel> modePanel;
	std::unique_ptr<PrintPanel> printPanel;
	//std::unique_ptr<FeedbackPanel> feedbackPanel;
	std::unique_ptr<PropertiesPanel> propertiesPanel;

};