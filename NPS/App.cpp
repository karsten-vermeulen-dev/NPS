#include <assert.h>
#include "App.h"
#include "Input.h"
#include "Screen.h"
#include "Shader.h"
#include "Text.h"
#include "UserInterface.h"

//======================================================================================================
App::App(State* initialState, int& errorCode)
{
	appState.reset(initialState);

	if (!Screen::Instance()->Initialize("Data/Config.ini"))
	{
		errorCode = -1;
		return;
	}

	if (!Screen::Instance()->InitializeUI())
	{
		errorCode = -1;
		return;
	}

	if (!Shader::Initialize())
	{
		errorCode = -1;
		return;
	}

	if (!Text::Initialize())
	{
		errorCode = -1;
		return;
	}
}
//======================================================================================================
App::~App()
{
	Text::Shutdown();
	Shader::Shutdown();
	Screen::Instance()->ShutdownUI();
	Screen::Instance()->Shutdown();
}
//======================================================================================================
void App::Run()
{
	if (appState)
	{
		if (appState->OnEnter())
		{
			while (appState)
			{
				Screen::Instance()->Refresh();
				Input::Instance()->Update();

				State* nextState = appState->Update();

				appState->Render();

				Screen::Instance()->Present();

				if (nextState != appState.get())
				{
					appState->OnExit();
					appState.reset(nextState);

					if (appState)
					{
						appState->OnEnter();
					}
				}
			}
		}
	}
}