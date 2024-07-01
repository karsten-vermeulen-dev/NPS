#pragma once

#include <Windows.h>
#include <string>

class Utility
{

public:

	enum class Severity
	{
		Failure = MB_ICONERROR,
		Warning = MB_ICONWARNING,
		Default = MB_ICONINFORMATION
	};

	enum class Destination
	{
		LogFile,
		OutputWindow,
		WindowsMessageBox
	};

	static void SetWindowHandle(HWND windowHandle);

	static void Log(Destination destination,
		const std::string& message, Severity severity = Severity::Default);

private:

	static HWND windowHandle;

};