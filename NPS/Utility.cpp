#include <fstream>
#include "GLAD/gl.h"
#include "Utility.h"

HWND Utility::windowHandle{ nullptr };
//================================================================================================
void Utility::SetWindowHandle(HWND windowHandle)
{
	Utility::windowHandle = windowHandle;
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