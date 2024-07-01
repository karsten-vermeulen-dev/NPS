#pragma once

#include <string>
#include "GLAD/gl.h"
#include <glm.hpp>
#include <SDL.h>
#include <Windows.h>
#include "Shader.h"

//This code usually resides in the stdafx.h file and  
//sets the manifest so that the modern Windows controls  
//are used. If this is missing dialog boxes will look old 
#ifdef _UNICODE 
#if defined _M_IX86 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#elif defined _M_X64 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#else 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#endif 
#endif

class Screen
{

public:

	static Screen* Instance();

	bool InitializeUI();
	bool Initialize(const std::string& filename);

	const HWND GetWindowHandle();
	const SDL_Rect& GetViewport();
	const glm::ivec2& GetMainResolution();

	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	void CreateNDCView();
	void CreatePerspView(Shader& shader);

	void CreateFrameBuffer(const glm::ivec2& resolution);
	void SaveScreenToFile(const glm::ivec2& resolution, const std::string& filename, bool hasAlphaChannel);
	void DestroyFrameBuffer();

	void Refresh();
	void Present();

	void Shutdown();
	void ShutdownUI();

private:

	Screen() {}
	Screen(const Screen&);
	Screen& operator=(Screen&);

	SDL_Rect viewport{ 0, 0, 0, 0 };
	glm::ivec2 mainResolution{ 0 };
	glm::mat4 projectionMatrix{ 1.0f };

	HWND windowHandle{ nullptr };
	SDL_Window* window{ nullptr };
	SDL_GLContext context{ nullptr };

};