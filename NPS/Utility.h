#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

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

	static void ParseString(std::string& string,
		std::vector<std::string>& subStrings, char token);

	static bool LoadConfigFile(const std::string& filename,
		std::map<std::string, std::string>& dataMap);

	static void Log(Destination destination,
		const std::string& message, Severity severity = Severity::Default);

private:

	static HWND windowHandle;

};