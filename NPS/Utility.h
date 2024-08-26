#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

class Utility
{

public:

	//TODO - Change this if we are using imgui message boxes only
	enum class Severity
	{
		Failure = MB_ICONERROR,
		Warning = MB_ICONWARNING,
		Default = MB_ICONINFORMATION   
	};

	enum class Destination
	{
		LogFile,
		OutputWindow,      //TODO - Do we ever use this?
		WindowsMessageBox  //TODO - Do we require this? Use imgui message boxes instead
	};

	static void SetWindowHandle(HWND windowHandle);

	//TODO - consider passing vec2 here instead of scalar values
	static float ConvertToNDC(int value, float maxValue);

	static void ParseString(std::string& string,
		std::vector<std::string>& subStrings, char token);

	//We can save standard configuration data to or load data from a config file
	//For this we use a standard 'hash-table' method of storing the data which is
	//perfect for loading screen initialization data or saving raw plate design data
	
	static bool SaveConfigFile(const std::string& filename,
		std::map<std::string, std::string>& dataMap);

	static bool LoadConfigFile(const std::string& filename,
		std::map<std::string, std::string>& dataMap);

	//We also have files that store data in 'sections' ([...]) and each
	//section contains sub-data that needs to be stored in its own map
	//For this we use a 'std::map of maps' to store the data and there is
	//no need to save data of this nature to a file, this is 'read-only'

	static bool LoadDataFile(const std::string& filename,
		std::map<std::string, std::map<std::string, std::string>>& dataMap);

	static void Log(Destination destination,
		const std::string& message, Severity severity = Severity::Default);

	static const std::string WindowsSaveFile(const WCHAR* filterText);
	
	static const std::string WindowsOpenFile(const WCHAR* filterText);
	
private:

	static HWND windowHandle;

};