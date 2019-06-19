#pragma once
// InputHandler.h

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include <SFML\Window.hpp>

enum e_InputType 
{
	e_KEYBOARD,
	e_MOUSE
};

struct Key
{
	Key()
	{
		pressed = false;
		changed = false;
	}

	bool pressed;
	bool changed;
};

struct Keyboard 
{
	Keyboard() 
	{
	}
	std::vector<Key> key;
};

struct Mouse 
{
	Mouse() 
	{
		mouseWheel = 0;
		mousePosition.x = 0;
		mousePosition.y = 0;
		mouseInWindow = true;
	}
	std::vector<Key> button;
	sf::Vector2f mousePosition;
	int mouseWheel;
	bool mouseInWindow;

};

class InputHandler 
{
public:
	InputHandler();
	~InputHandler();

	void KeyResetChanged();
	void KeyPressed(e_InputType i, int k);
	void KeyReleased(e_InputType i, int k);

	void MouseWheelMoved(int value);
	void MouseMoved(sf::Vector2f position);
	void MouseInWindow(bool inWindow);

	Key* getKey(e_InputType i, int k);
	bool isKeyPressed(e_InputType i, int k);
	bool isKeyDown(e_InputType i, int k);
	bool isKeyReleased(e_InputType i, int k);
	sf::Vector2f getMousePosition();
	int getMouseWheel();
	bool getMouseInWindow();

protected:

private:
	void CreateKeys();
	void CreateMouse();

	Keyboard keyboard;
	Mouse mouse;
};

#endif