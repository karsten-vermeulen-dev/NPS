#pragma once
#include <memory>
#include "State.h"

class App
{

public:

	App(State* initialState, int& errorCode);
	~App();
	void Run();

private:

	std::unique_ptr<State> appState;

};