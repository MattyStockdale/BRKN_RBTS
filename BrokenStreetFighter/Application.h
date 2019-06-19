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
	void Restart();
	void InitialiseText();
	void ManageEvents(sf::RenderWindow* window);
	void Process(sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	void LocationComparison();
	void DetectCollisions();
	void SetText(sf::Text*text, sf::Font* font, const sf::String string, sf::Color color, int charSize, sf::Text::Style style = sf::Text::Regular, sf::Vector2f position = sf::Vector2f(0, 0));

	bool GetCollision(Player p1, Player p2);
	bool gameStarted;

	InputHandler hInput;

	GameObj Background;
	GameObj Player1HealthbarSprite;
	GameObj Player2HealthbarSprite;

	Player Players[PLAYER_COUNT];

	e_GameState currentState;
	e_WinState winState;

	sf::Clock StartTimer;
	sf::Clock ControlChangeTimer;
	sf::Clock DeathTimer;
	sf::Clock frameClock;

	sf::Time frameTime;

	sf::Font MainFont;

	sf::Text GameLogo;
	sf::Text StartScreenInfo1;
	sf::Text TeamText;
	sf::Text CountdownText;
	sf::Text WinnerText;
	sf::Text Player1Health;
	sf::Text Player2Health;
};

#endif