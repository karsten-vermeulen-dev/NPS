#include <assert.h>
#include <fstream>
#include <map>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <gtc\matrix_transform.hpp>
#include <SDL_image.h>
#include <SDL_syswm.h>
#include "Screen.h"
#include "Utility.h"

//======================================================================================================
Screen* Screen::Instance()
{
	static Screen* screen = new Screen();
	return screen;
}
//======================================================================================================
bool Screen::InitializeUI()
{
	//Did you forget to call Initialize()?
	assert(window && context);

	ImGui::CreateContext();
	if (!ImGui_ImplOpenGL3_Init("#version 400"))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error initializing UI", Utility::Severity::Failure);
		return false;
	}

	if (!ImGui_ImplSDL2_InitForOpenGL(window, context))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error initializing UI", Utility::Severity::Failure);
		return false;
	}

	return true;
}
//======================================================================================================
bool Screen::Initialize(const std::string& filename)
{
	std::map<std::string, std::string> dataMap;

	if (!Utility::LoadConfigFile(filename, dataMap))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error opening config file '" + filename + "'", Utility::Severity::Failure);
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"SDL did not initialize properly.", Utility::Severity::Failure);
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	auto profile = std::stoi(dataMap["Core"]) == 1 ? SDL_GL_CONTEXT_PROFILE_CORE
		: SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, std::stoi(dataMap["Major"]));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, std::stoi(dataMap["Minor"]));

	window = SDL_CreateWindow(dataMap["Name"].c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		std::stoi(dataMap["Width"]),
		std::stoi(dataMap["Height"]),
		SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);

	if (!window)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Game window could not be created.", Utility::Severity::Failure);
		return false;
	}

	context = SDL_GL_CreateContext(window);

	if (!context)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"OpenGL context could not be created. "
			"The context is either invalid or not supported by your graphics card",
			Utility::Severity::Failure);
		return false;
	}

	if (!gladLoaderLoadGL())
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"GLAD could not be initialized", Utility::Severity::Failure);
		return false;
	}

	SDL_GetWindowSize(window, &mainResolution.x, &mainResolution.y);

	SDL_SysWMinfo systemInfo;
	SDL_VERSION(&systemInfo.version);
	SDL_GetWindowWMInfo(window, &systemInfo);
	windowHandle = systemInfo.info.win.window;

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}
//======================================================================================================
const HWND Screen::GetWindowHandle()
{
	return windowHandle;
}
//======================================================================================================
const glm::ivec2& Screen::GetMainResolution()
{
	return mainResolution;
}
//======================================================================================================
const SDL_Rect& Screen::GetViewport()
{
	return viewport;
}
//======================================================================================================
void Screen::CreateNDCView()
{
	glDisable(GL_DEPTH_TEST);
}
//======================================================================================================
void Screen::CreatePerspView(Shader& shader)
{
	glEnable(GL_DEPTH_TEST);
	auto aspectRatio = (mainResolution.x) / static_cast<GLfloat>(mainResolution.y);
	projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	shader.SendData("projection", projectionMatrix);
}
//======================================================================================================
void Screen::CreateFrameBuffer(const glm::ivec2& resolution)
{
	GLuint FBO = 0;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//Create an empty texture object to render to
	//We bind this texture object to the FBO because
	//frame buffers cannot be used on their own
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		resolution.x,
		resolution.y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Bind the texture object with our custom frame buffer
	//When using the frame buffer, the rendering target will be the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
}
//======================================================================================================
void Screen::SaveScreenToFile(const glm::ivec2& resolution,
	const std::string& filename,
	bool hasAlphaChannel)
{
	//We need mask filters for correct color values
	//when creating the SDL_Surface image object
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = (hasAlphaChannel) ? 0xff000000 : 0x00000000;

	SDL_Surface* rawImage = SDL_CreateRGBSurface(0,
		resolution.x,
		resolution.y,
		32,
		rmask,
		gmask,
		bmask,
		amask);

	if (!rawImage)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Cannot create image resource. Possibly out of memory.");
	}

	else
	{
		//Reading data from the currently bound frame buffer
		glReadPixels(0,
			0,
			resolution.x,
			resolution.y,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			rawImage->pixels);

		//Create a new surface to contain the flipped pixels
		//We remove 1 pixel from the width and height to avoid black lines
		SDL_Surface* newImage = SDL_CreateRGBSurface(0,
			resolution.x - 1,
			resolution.y - 1,
			32,
			rmask,
			gmask,
			bmask,
			amask);

		if (!newImage)
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Cannot create image resource. Possibly out of memory.");
		}

		//Flip the pixel data so that the image is not stored upside down
		else
		{
			GLuint* pixelsNewImage = static_cast<GLuint*>(newImage->pixels);
			GLuint* pixelsRawImage = static_cast<GLuint*>(rawImage->pixels);

			for (int column = 0; column < rawImage->w - 1; column++)
			{
				for (int row = 0; row < rawImage->h - 1; row++)
				{
					pixelsNewImage[row * newImage->w + column] =
						pixelsRawImage[(rawImage->h - 1 - row) * rawImage->w + column];
				}
			}

			if (hasAlphaChannel)
			{
				if (IMG_SavePNG(newImage, filename.c_str()) == -1)
				{
					Utility::Log(Utility::Destination::WindowsMessageBox, "Error saving to file");
				}
			}

			else
			{
				if (SDL_SaveBMP(newImage, filename.c_str()) == -1)
				{
					Utility::Log(Utility::Destination::WindowsMessageBox, "Error saving to file");
				}
			}

			SDL_FreeSurface(newImage);
		}

		SDL_FreeSurface(rawImage);
	}
}
//======================================================================================================
void Screen::DestroyFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//======================================================================================================
void Screen::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glScissor(x, y, width, height);
	glViewport(x, y, width, height);
	viewport = { x, y, width, height };
}
//======================================================================================================
void Screen::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}
//======================================================================================================
void Screen::Refresh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//======================================================================================================
void Screen::Present()
{
	SDL_GL_SwapWindow(window);
}
//======================================================================================================
void Screen::Shutdown()
{
	if (window && context)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}
//======================================================================================================
void Screen::ShutdownUI()
{
	if (window && context)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
}