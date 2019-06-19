// BrokenStreetFighter
// ADGS Game Jam - October 2015
// Dean, Matt & Sidney
// ===============================
// Application.h

#ifndef APPLICATION_H
#define APPLICATION_H

#include "ApplicationDefines.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "AnimatedSprite.hpp"
#include "Player.h"

class Application {
public:
	Application();
	~Application();

	void Run();

protected:


private:
	void Initialise();
	void InitialiseText();

	void ManageEvents(sf::RenderWindow* window);
	void Process(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void LocationComparison();
	void DetectCollisions();
	bool GetCollision(Player p1, Player p2);

	void SetText(sf::Text* text, sf::Font font, const sf::String string, sf::Color color, sf::Vector2f position, float charSize);

	InputHandler hInput;
	GameObj Background;
	Player Players[PLAYER_COUNT];

	e_GameState currentState;
	e_WinState winState;

	bool fullscreen;
	
	sf::Clock StartTimer;
	bool gameStarted;

	sf::Clock ControlChangeTimer;
	sf::Clock DeathTimer;

	sf::Clock frameClock;
	sf::Time frameTime;

	sf::Font MainFont;
	sf::Text GameLogo;
	sf::Text StartScreenInfo1;
	sf::Text TeamText;

	sf::Text Player1Txt;
	sf::Text Player1Status;

	sf::Text Player2Txt;
	sf::Text Player2Status;

	sf::Text CountdownText;
	sf::Text TimerText;
	sf::Text Player1Text;
	sf::Text Player2Text;

	sf::Text WinnerText;
	sf::Text Player1Health;
	sf::Text Player2Health;
};

#endif