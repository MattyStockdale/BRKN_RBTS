// InputHandler.cpp

#include "InputHandler.h"

InputHandler::InputHandler()
{
	CreateKeys();
	CreateMouse();
}

InputHandler::~InputHandler() 
{
}

void InputHandler::CreateKeys() 
{
	for(int k = 0; k < sf::Keyboard::KeyCount; ++k) {
		keyboard.key.push_back(Key());
	}
}

void InputHandler::CreateMouse() 
{
	for(int b = 0; b < sf::Mouse::ButtonCount; ++b) {
		mouse.button.push_back(Key());
	}
}



void InputHandler::KeyResetChanged() 
{
	for(unsigned int k = 0; k < keyboard.key.size(); ++k) 
	{
		keyboard.key.at(k).changed = false;
	}
	for(unsigned int b = 0; b < mouse.button.size(); ++b)
	{
		mouse.button.at(b).changed = false;
	}
	mouse.mouseWheel = 0;
}

void InputHandler::KeyPressed(e_InputType i, int k) 
{
	if(k < keyboard.key.size()) 
	{
		switch(i) 
		{
			case e_KEYBOARD:
				if(keyboard.key.at(k).pressed == false) 
				{
					keyboard.key.at(k).changed = true;
				}
				keyboard.key.at(k).pressed = true;
				break;
			case e_MOUSE:
				if(mouse.button.at(k).pressed == false) 
				{
					mouse.button.at(k).changed = true;
				}
				mouse.button.at(k).pressed = true;
				break;
		}
	}
}

void InputHandler::KeyReleased(e_InputType i, int k) 
{
	if(k < keyboard.key.size()) 
	{
		switch(i) 
		{
			case e_KEYBOARD:
				if(keyboard.key.at(k).pressed == true)
				{
					keyboard.key.at(k).changed = true;
				}
				keyboard.key.at(k).pressed = false;
				break;
			case e_MOUSE:
				if(mouse.button.at(k).pressed == true)
				{
					mouse.button.at(k).changed = true;
				}
				mouse.button.at(k).pressed = false;
				break;
		}
	}
}

void InputHandler::MouseWheelMoved(int value) 
{
	mouse.mouseWheel = value;
}

void InputHandler::MouseMoved(sf::Vector2f position) 
{
	mouse.mousePosition = position;
}

void InputHandler::MouseInWindow(bool inWindow) 
{
	mouse.mouseInWindow = inWindow;
}


Key* InputHandler::getKey(e_InputType i, int k) 
{
	switch(i) 
	{
		case e_KEYBOARD:
			return &keyboard.key.at(k);
			break;
		case e_MOUSE:
			return &mouse.button.at(k);
			break;
	}
}

bool InputHandler::isKeyPressed(e_InputType i, int k) 
{
	switch(i) {
		case e_KEYBOARD:
			if(keyboard.key.at(k).changed == true) 
			{
				if(keyboard.key.at(k).pressed == true) 
				{
					return true;
				}
			}
			return false;
			break;
		case e_MOUSE:
			if(mouse.button.at(k).changed == true)
			{
				if(mouse.button.at(k).pressed == true) 
				{
					return true;
				}
			}
			return false;
			break;
	}
}

bool InputHandler::isKeyDown(e_InputType i, int k) 
{
	switch(i) {
		case e_KEYBOARD:
			if(keyboard.key.at(k).pressed == true) 
			{
				return true;
			}
			return false;
			break;
		case e_MOUSE:
			if(mouse.button.at(k).pressed == true)
			{
				return true;
			}
			return false;
			break;
	}
}

bool InputHandler::isKeyReleased(e_InputType i, int k)
{
	switch(i) 
	{
		case e_KEYBOARD:
			if(keyboard.key.at(k).changed == true)
			{
				if(keyboard.key.at(k).pressed == false) 
				{
					return true;
				}
			}
			return false;
			break;
		case e_MOUSE:
			if(mouse.button.at(k).changed == true) 
			{
				if(mouse.button.at(k).pressed == false) 
				{
					return true;
				}
			}
			return false;
			break;
	}
}

sf::Vector2f InputHandler::getMousePosition() 
{
	return mouse.mousePosition;
}

int InputHandler::getMouseWheel() 
{
	return mouse.mouseWheel;
}

bool InputHandler::getMouseInWindow() 
{
	return mouse.mouseInWindow;
}