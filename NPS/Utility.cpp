#include <assert.h>
#include <fstream>
#include "GLAD/gl.h"
#include "Utility.h"
#include "Screen.h"

HWND Utility::windowHandle{ nullptr };
//================================================================================================
void Utility::SetWindowHandle(HWND windowHandle)
{
	Utility::windowHandle = windowHandle;
}
//======================================================================================================
void Utility::ParseString(std::string& string, std::vector<std::string>& subStrings, char token)
{
	if (!string.empty())
	{
		size_t start = 0;
		size_t end = 0;

		while (end != std::string::npos)
		{
			end = string.find(token, start);
			if ((end - start) > 0)
			{
				subStrings.push_back(string.substr(start, end - start));
			}
			start = end + 1;
		}
	}
}
//======================================================================================================
bool Utility::LoadConfigFile(const std::string& filename, std::map<std::string, std::string>& dataMap)
{
	std::fstream file(filename, std::ios_base::in);

	if (!file.is_open())
	{
		return false;
	}

	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);
		std::vector<std::string> subStrings;
		ParseString(line, subStrings, '=');

		if (!subStrings.empty())
		{
			dataMap[subStrings[0]] = subStrings[1];
		}
	}

	file.close();
	return true;
}
//======================================================================================================
void Utility::Log(Destination destination, const std::string& message, Severity severity)
{
	if (!message.empty())
	{
		if (destination == Destination::WindowsMessageBox)
		{
			MessageBox(windowHandle,
				reinterpret_cast<LPCWSTR>(std::wstring(message.begin(), message.end()).c_str()),
				L"Log", static_cast<GLuint>(severity) | MB_OK);
		}

		else if (destination == Destination::OutputWindow)
		{
			std::string finalMessage;

			if (severity == Severity::Failure)
			{
				finalMessage = "[FAILURE] " + message + "\n";
			}

			else if (severity == Severity::Warning)
			{
				finalMessage = "[WARNING] " + message + "\n";
			}

			else
			{
				finalMessage = message + "\n";
			}

			OutputDebugString(reinterpret_cast<LPCWSTR>
				(std::wstring(finalMessage.begin(), finalMessage.end()).c_str()));
		}

		else if (destination == Destination::LogFile)
		{
			std::string finalMessage;
			std::fstream file("Data/Output.log", std::ios_base::out | std::ios_base::app);

			if (severity == Severity::Failure)
			{
				finalMessage = "[FAILURE] " + message + "\n";
			}

			else if (severity == Severity::Warning)
			{
				finalMessage = "[WARNING] " + message + "\n";
			}

			else
			{
				finalMessage = message + "\n";
			}

			file << finalMessage;
			file.close();
		}
	}
}
//======================================================================================================
const std::string Utility::WindowsOpenFile(const WCHAR* filterText)
{
	OPENFILENAME openDialog;
	char filename[260] = { '\0' };

	ZeroMemory(&openDialog, sizeof(openDialog));
	openDialog.lStructSize = sizeof(openDialog);
	openDialog.hwndOwner = Screen::Instance()->GetWindowHandle();
	openDialog.lpstrFile = (LPWSTR)filename;
	openDialog.nMaxFile = sizeof(filename);
	openDialog.lpstrFilter = filterText;
	openDialog.nFilterIndex = 1;
	openDialog.lpstrFileTitle = nullptr;
	openDialog.nMaxFileTitle = 0;
	openDialog.lpstrInitialDir = nullptr;
	openDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//This calls the actual Windows dialog box and returns a '1' or '0' based 
	//on if a valid file has been found. If there are any issues, they will be 
	//processed within this function so no need for our own error messages/boxes
	if (GetOpenFileName(&openDialog) == TRUE)
	{
		auto filenameWS = std::wstring(openDialog.lpstrFile);
		return std::string(filenameWS.begin(), filenameWS.end());
	}

	//The calling code can use this to check if a valid file was found
	return "";
}