#pragma once
#include <SDL.h>
#include <glm.hpp>

typedef const Uint8* KeyState;

class Input
{

public:

	enum class ButtonState
	{
		Up,
		Down
	};

	static Input* Instance();

	char GetKeyUp() const;
	char GetKeyDown() const;
	KeyState GetKeyStates() const;

	bool IsXClicked() const;
	bool IsKeyPressed() const;
	bool IsWindowResized() const;

	bool IsLeftButtonClicked() const;
	bool IsRightButtonClicked() const;
	bool IsMiddleButtonClicked() const;

	const glm::ivec2& GetMouseWheel() const;
	const glm::ivec2& GetMouseMotion() const;
	const glm::ivec2& GetMousePosition() const;

	void Update();

private:

	Input() {}
	Input(const Input&);
	Input& operator=(const Input&);

	char keyUp{ 0 };
	char keyDown{ 0 };

	bool isXClicked{ false };
	bool isKeyPressed{ false };
	bool isWindowResized{ false };

	KeyState keyStates{ nullptr };
	SDL_Cursor* cursor{ nullptr };

	glm::ivec2 mouseWheel{ 0 };
	glm::ivec2 mouseMotion{ 0 };
	glm::ivec2 mousePosition{ 0 };

	bool isLeftButtonClicked{ false };
	bool isRightButtonClicked{ false };
	bool isMiddleButtonClicked{ false };

};