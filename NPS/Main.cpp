#include <SDL.h>
#include "App.h"
#include "Design.h"

int main(int argc, char* args[])
{
	auto errorCode{ 0 };
	App app(new Design, errorCode);

	if (errorCode != -1)
	{
		app.Run();
	}

	return 0;
}