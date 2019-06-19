// Application.cpp
#include "Application.h"

Application::Application() 
{
	Initialise();
	InitialiseText();
}

Application::~Application() 
{
}

void Application::Run() 
{
	sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "BRKN RBTS", sf::Style::Default);
	window.setFramerateLimit(60);

	// Game loop
	while(window.isOpen()) 
	{
		frameTime = frameClock.restart();
		ManageEvents(&window);		// Get events (input, etc.)
		Process(&window);			// Handle game logic, main mechanics
		Render(&window);			// Display graphics
	}
}

void Application::Initialise() 
{
	currentState = e_START;
	winState = e_ONGOING;
	gameStarted = false;

	Background.sprite.setPosition(0, 0);
	Background.SetTexture("bin/AGDSbackground.png");
	Background.sprite.setTexture(Background.texture);

	Player1HealthbarSprite.SetTexture("bin/AGDSjam_player1bar.png");
	Player1HealthbarSprite.sprite.setTexture(Player1HealthbarSprite.texture);

	Player2HealthbarSprite.SetTexture("bin/AGDSjam_player2bar.png");
	Player2HealthbarSprite.sprite.setTexture(Player2HealthbarSprite.texture);
	Player2HealthbarSprite.sprite.setPosition(RESOLUTION_X - 800, 0);

	Players[0].Initialise(&hInput, 0, sf::Vector2f(PLAYER_1_START, FLOOR_Y),false);
	Players[1].Initialise(&hInput, 1, sf::Vector2f(PLAYER_2_START, FLOOR_Y),true);
}

void Application::Restart()
{
	currentState = e_START;
	winState = e_ONGOING;
	gameStarted = false;
	Players[0].Initialise(&hInput, 0, sf::Vector2f(PLAYER_1_START, FLOOR_Y), false,true);
	Players[1].Initialise(&hInput, 1, sf::Vector2f(PLAYER_2_START, FLOOR_Y), true,true);
}

void Application::InitialiseText() 
{
	if(!MainFont.loadFromFile("bin/Fonts/joystix_monospace.ttf")) 
	{
		std::cout << "Failed to Load Spritesheet at bin / Fonts / joystix_monospace.ttf"  << std::endl;
	}

	// INTRO
	SetText(&GameLogo, &MainFont, "BRKN RBTS", sf::Color::White, 100);
	GameLogo.setPosition((RESOLUTION_X / 2) - (GameLogo.getGlobalBounds().width / 2), RESOLUTION_Y / 3);
	SetText(&TeamText, &MainFont, "Team Placeholder", sf::Color::White, 50);
	TeamText.setPosition((RESOLUTION_X / 2) - (TeamText.getGlobalBounds().width / 2), RESOLUTION_Y * 0.5);
	SetText(&StartScreenInfo1, &MainFont, "Press <return> to start", sf::Color::White, 25);
	StartScreenInfo1.setPosition((RESOLUTION_X / 2) - (StartScreenInfo1.getGlobalBounds().width / 2), RESOLUTION_Y * 0.8);


	// IN GAME
	SetText(&CountdownText, &MainFont, "3", sf::Color::Black, 100, sf::Text::Bold);
	SetText(&Player1Health, &MainFont, "100", sf::Color::White, 90, sf::Text::Bold);
	SetText(&Player2Health, &MainFont, "100", sf::Color::White, 90, sf::Text::Bold);

	// END SCREEN
	SetText(&WinnerText, &MainFont, "A winner is you!",sf::Color::White,100);
	WinnerText.setPosition((RESOLUTION_X / 2) - (WinnerText.getGlobalBounds().width / 2), RESOLUTION_Y / 2); 
}

void Application::SetText(sf::Text*text, sf::Font* font, const sf::String string, sf::Color color, int charSize,sf::Text::Style style, sf::Vector2f position) 
{
	text->setFont(*font);
	text->setFillColor(color);
	text->setCharacterSize(charSize);
	text->setString(string);
	text->setPosition(position);
	text->setStyle(style);
}

void Application::ManageEvents(sf::RenderWindow* window) 
{
	sf::Event event;

	hInput.KeyResetChanged();
	// Sifts through all events
	while(window->pollEvent(event)) 
	{
		// Check event type
		switch(event.type) 
		{
			case sf::Event::Closed:					// Window closed
				window->close();
				break;
			case sf::Event::KeyPressed:				// Keyboard down
				hInput.KeyPressed(e_KEYBOARD, event.key.code);
				break;
			case sf::Event::KeyReleased:			// Keyboard up
				hInput.KeyReleased(e_KEYBOARD, event.key.code);
				break;
			case sf::Event::MouseButtonPressed:		// Mouse down
				hInput.KeyPressed(e_MOUSE, event.mouseButton.button);
				break;
			case sf::Event::MouseButtonReleased:	// Mouse up
				hInput.KeyReleased(e_MOUSE, event.mouseButton.button);
				break;
			case sf::Event::MouseMoved:				// Mouse moved
				hInput.MouseMoved(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
				break;
			case sf::Event::MouseWheelMoved:		// Mouse wheel scrolled
				hInput.MouseWheelMoved(event.mouseWheel.delta);
				break;
			case sf::Event::MouseEntered:			// Mouse entered window
				hInput.MouseInWindow(true);
				break;
			case sf::Event::MouseLeft:				// Mouse left window
				hInput.MouseInWindow(false);
				break;
		}
	}
}

void Application::Process(sf::RenderWindow* window) 
{
	switch(currentState) 
	{
		case e_START:
			// RETURN - Start game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) 
			{
				currentState = e_INGAME;
				StartTimer.restart();
			}
			// ESCAPE - Close game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Escape)) 
			{
				window->close();
			}
			break;
		case e_INGAME:
			// Check win state
			if(winState != e_ONGOING) 
			{
				if (Players[0].dead)
				{
					Players[0].Dead();
					Players[1].animatedSprite.stop();
				}
				else if (Players[1].dead) 
				{
					Players[1].Dead();
				}
				if (DeathTimer.getElapsedTime() >= sf::seconds(1.5))
				{
					currentState = e_END;
				}
				
			} 
			else 
			{
				if(gameStarted == false) 
				{		// If game not started yet
					if(StartTimer.getElapsedTime() >= sf::seconds(START_COUNTDOWN)) 
					{		
						// If countdown has finished
						gameStarted = true;
						ControlChangeTimer.restart();
					}
				} 
				else 
				{
					// Update player classes
					for(int i = 0; i < PLAYER_COUNT; ++i) 
					{
						Players[i].Update(&hInput);
					}
					
					LocationComparison();
					for(int i = 0; i < PLAYER_COUNT; ++i) 
					{
						Players[i].Update(&hInput);
					}

					// Collision detection
					DetectCollisions();

					if(ControlChangeTimer.getElapsedTime() >= sf::seconds(CONTROL_CHANGE_DELAY))
					{
						for(int i = 0; i < PLAYER_COUNT; ++i) 
						{
							Players[i].ChangeControls(&hInput);
							ControlChangeTimer.restart();
						}
					}

					// Check win condition
					if(Players[0].dead) 
					{
						winState = e_PLAYER_1_WIN;
						DeathTimer.restart();
					} else if(Players[1].dead) 
					{
						winState = e_PLAYER_0_WIN;
						DeathTimer.restart();
					}
					break;
				}
			}
		case e_END:
			// ESCAPE - Close game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Escape)) 
			{
				window->close();
			}

			// RETURN - Start game again
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) 
			{
				Restart();
			}
			break;
	}
}

void Application::Render(sf::RenderWindow* window) 
{
	window->clear();

	switch(currentState) 
	{
		case e_START:
			// Render UI
			window->draw(GameLogo);
			window->draw(TeamText);
			window->draw(StartScreenInfo1);
			break;

		case e_INGAME:
			// Render background
			window->draw(Background.sprite);
			window->draw(Player2HealthbarSprite.sprite);
			window->draw(Player1HealthbarSprite.sprite);
			
			// Render foreground
			for(int i = 0; i < PLAYER_COUNT; ++i) 
			{
				Players[i].animatedSprite.update(frameTime);
				window->draw(Players[i].animatedSprite);
			}
			
			// Render UI
			if(!gameStarted) 
			{
				int count = 4 - StartTimer.getElapsedTime().asSeconds();
				CountdownText.setString(std::to_string(count));
				CountdownText.setPosition((RESOLUTION_X / 2) - (CountdownText.getGlobalBounds().width / 2), RESOLUTION_Y * 0.6);
				if(count != 0) 
				{
					window->draw(CountdownText);
				}
			}

			Player1Health.setString(std::to_string(Players[0].health));
			Player2Health.setString(std::to_string(Players[1].health));
			Player2Health.setPosition((RESOLUTION_X - (Player2Health.getGlobalBounds().width)) - 95, 20);
			Player1Health.setPosition(70, 20);

			window->draw(Player1Health);
			window->draw(Player2Health);
			break;

		case e_END:
			window->draw(WinnerText);
			break;
	}
	window->display();
}

void Application::LocationComparison()
{
	float x_a = Players[0].Position.x;
	float x_b = Players[1].Position.x;

	if (x_a > x_b && (Players[0].flipped == false && Players[1].flipped == true)){
		Players[0].flipped = true;
		Players[1].flipped = false;
	}
	else if (x_a < x_b && (Players[0].flipped == true && Players[1].flipped == false)){
		Players[0].flipped = false;
		Players[1].flipped = true;
	}
}

void Application::DetectCollisions() 
{
	if(GetCollision(Players[0], Players[1])) 
	{		
		// If collision
		if(Players[0].isAttacking && Players[0].attackType != e_NO_ATTACK && (Players[0].AttackTimer.getElapsedTime() >= sf::seconds(0.35f) && Players[0].AttackTimer.getElapsedTime() <= sf::seconds(0.4f))) {
			// Damage enemy player
			Players[1].DepleteHealth(Players[0].attackType);
			// Knock player back
			if(Players[0].boundingBox.min.x < Players[1].boundingBox.min.x) 
			{
				Players[1].Knockback(e_RIGHT, Players[1].attackType);
			} 
			else 
			{
				Players[1].Knockback(e_LEFT, Players[1].attackType);
			}
			Players[0].isAttacking = false;
		}
		if(Players[1].isAttacking && Players[1].attackType != e_NO_ATTACK && Players[1].AttackTimer.getElapsedTime() >= sf::seconds(0.35f)) 
		{
			// Damage enemy player
			Players[0].DepleteHealth(Players[1].attackType);
			// Knock player back
			if(Players[1].boundingBox.min.x < Players[0].boundingBox.min.x) 
			{
				Players[0].Knockback(e_RIGHT, Players[0].attackType);
			} 
			else 
			{
				Players[0].Knockback(e_LEFT, Players[0].attackType);
			}
			Players[1].isAttacking = false;
		}
	}
}

bool Application::GetCollision(Player p1, Player p2)
{
	if(p1.boundingBox.min.x > p2.boundingBox.max.x)	return false;
	if(p1.boundingBox.max.x < p2.boundingBox.min.x)	return false;
	if(p1.boundingBox.min.y > p2.boundingBox.max.y)	return false;
	if(p1.boundingBox.max.y < p2.boundingBox.min.y)	return false;

	return true;
}

