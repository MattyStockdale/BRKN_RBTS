// BrokenStreetFighter
// ADGS Game Jam - October 2015
// Dean, Matt & Sidney
// ===============================
// Application.cpp

#include "Application.h"

Application::Application() {
	Initialise();
	InitialiseText();
}

Application::~Application() {
}

void Application::Run() {
	sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "Lol", sf::Style::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y), "PLACEHOLDER");
	window.setFramerateLimit(60);

	// Game loop
	while(window.isOpen()) {
		frameTime = frameClock.restart();
		ManageEvents(&window);		// Get events (input, etc.)
		Process(&window);			// Handle game logic, main mechanics
		Render(&window);			// Display graphics
	}
}

void Application::Initialise() {
	currentState = e_START;
	winState = e_ONGOING;
	gameStarted = false;
	Background.SetupStaticSprite(sf::Vector2f(10,10));
	Background.background.setPosition(-20, 0);
	Players[0].SetUpSprite();
	Players[0].flipped = false;
	Players[0].Initialise(&hInput, 0, sf::Vector2f(PLAYER_1_START, FLOOR_Y));
	Players[1].SetUpSprite();
	Players[1].PlayerTexFlipped();
	Players[1].flipped = true;
	Players[1].Initialise(&hInput, 1, sf::Vector2f(PLAYER_2_START, FLOOR_Y));
}

void Application::InitialiseText() {
	if(!MainFont.loadFromFile("bin/Fonts/joystix_monospace.ttf")) {
		// FREAK OUT
	}

	// START SCREEN
	GameLogo.setFont(MainFont);
	GameLogo.setColor(sf::Color::White);
	GameLogo.setCharacterSize(75);
	GameLogo.setString("BRKN RBTS");
	GameLogo.setPosition((RESOLUTION_X / 2) - (GameLogo.getGlobalBounds().width / 2), RESOLUTION_Y / 4);

	TeamText.setFont(MainFont);
	TeamText.setColor(sf::Color::White);
	TeamText.setCharacterSize(50);
	TeamText.setString("Team Placeholder");
	TeamText.setPosition((RESOLUTION_X / 2) - (TeamText.getGlobalBounds().width / 2), RESOLUTION_Y * 0.6);

	StartScreenInfo1.setFont(MainFont);
	StartScreenInfo1.setColor(sf::Color::White);
	StartScreenInfo1.setCharacterSize(25);
	StartScreenInfo1.setString("Press <return> to start.");
	StartScreenInfo1.setPosition((RESOLUTION_X / 2) - (StartScreenInfo1.getGlobalBounds().width / 2), RESOLUTION_Y * 0.8);

	// IN LOBBY
	Player1Txt.setFont(MainFont);
	Player1Txt.setColor(sf::Color::White);
	Player1Txt.setCharacterSize(50);
	Player1Txt.setString("Player 1, ARE YOU READY?");
	Player1Txt.setPosition((RESOLUTION_X / 2) - (Player1Txt.getGlobalBounds().width / 2), 100);

	Player1Status.setFont(MainFont);
	Player1Status.setColor(sf::Color::Red);
	Player1Status.setCharacterSize(50);
	Player1Status.setString("READY!!");
	Player1Status.setPosition((RESOLUTION_X / 2) - (Player1Status.getGlobalBounds().width / 2), 200);

	Player2Txt.setFont(MainFont);
	Player2Txt.setColor(sf::Color::White);
	Player2Txt.setCharacterSize(50);
	Player2Txt.setString("Player 2, ARE YOU READY?");
	Player2Txt.setPosition((RESOLUTION_X / 2) - (Player2Txt.getGlobalBounds().width / 2),434);

	Player2Status.setFont(MainFont);
	Player2Status.setColor(sf::Color::Red);
	Player2Status.setCharacterSize(50);
	Player2Status.setString("READY!!");
	Player2Status.setPosition((RESOLUTION_X / 2) - (Player2Status.getGlobalBounds().width / 2), 534);

	// IN GAME
	CountdownText.setFont(MainFont);
	CountdownText.setColor(sf::Color::Black);
	CountdownText.setCharacterSize(100);
	CountdownText.setString("3");
	CountdownText.setStyle(sf::Text::Bold);

	TimerText.setFont(MainFont);
	TimerText.setColor(sf::Color::Black);
	TimerText.setCharacterSize(40);
	TimerText.setString("00");
	TimerText.setPosition((RESOLUTION_X / 2) - (TimerText.getGlobalBounds().width / 2), 0);
	TimerText.setStyle(sf::Text::Bold);

	Player1Text.setFont(MainFont);
	Player1Text.setColor(sf::Color::Black);
	Player1Text.setCharacterSize(25);
	Player1Text.setString("Player 1");
	Player1Text.setPosition(10, 0);
	Player1Text.setStyle(sf::Text::Bold);

	Player2Text.setFont(MainFont);
	Player2Text.setColor(sf::Color::Black);
	Player2Text.setCharacterSize(40);
	Player2Text.setString("100");
	Player2Text.setPosition((RESOLUTION_X - (Player2Text.getGlobalBounds().width)), 0);
	Player2Text.setStyle(sf::Text::Bold);

	Player1Health.setFont(MainFont);
	Player1Health.setColor(sf::Color::White);
	Player1Health.setCharacterSize(40);
	Player1Health.setString("100");
	Player1Health.setPosition(50, 50);
	Player1Health.setStyle(sf::Text::Bold);

	Player2Health.setFont(MainFont);
	Player2Health.setColor(sf::Color::White);
	Player2Health.setCharacterSize(40);
	Player2Health.setString("100");
	Player2Health.setPosition((RESOLUTION_X - (Player2Health.getGlobalBounds().width)) - 50, 50);
	Player2Health.setStyle(sf::Text::Bold);

	// END SCREEN
	WinnerText.setFont(MainFont);
	WinnerText.setColor(sf::Color::White);
	WinnerText.setCharacterSize(100);
	WinnerText.setString("A winner is you!");
	WinnerText.setPosition((RESOLUTION_X / 2) - (WinnerText.getGlobalBounds().width / 2), RESOLUTION_Y / 2);
}

void Application::SetText(sf::Text* text, sf::Font font, const sf::String string, sf::Color color, sf::Vector2f position, float charSize) {
	text->setFont(font);
	text->setColor(color);
	text->setCharacterSize(charSize);
	text->setString(string);
	text->setPosition(position);
}

void Application::ManageEvents(sf::RenderWindow* window) {
	sf::Event event;

	hInput.KeyResetChanged();
	// Sifts through all events
	while(window->pollEvent(event)) {
		// Check event type
		switch(event.type) {
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

void Application::Process(sf::RenderWindow* window) {
	switch(currentState) {
		case e_START:
			// RETURN - Start game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) {
				currentState = e_LOBBY;
				StartTimer.restart();
			}
			// ESCAPE - Close game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Escape)) {
				window->close();
			}
			break;
		case e_LOBBY:
			// RETURN - Start game
			if (hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) {
				currentState = e_INGAME;
				StartTimer.restart();
			}
			break;
		case e_INGAME:
			// Check win state
			if(winState != e_ONGOING) {
				if (Players[0].dead){
					Players[0].DEAD();
					Players[1].animatedSprite.stop();
				}
				else if (Players[1].dead) {
					Players[1].DEAD();
				}
				if (DeathTimer.getElapsedTime() >= sf::seconds(1.5)){
					currentState = e_END;
				}
				
			} else {
				if(gameStarted == false) {		// If game not started yet
					if(StartTimer.getElapsedTime() >= sf::seconds(START_COUNTDOWN)) {		// If countdown has finished
						gameStarted = true;
						ControlChangeTimer.restart();
					}
				} else {
					// Update player classes
					for(int i = 0; i < PLAYER_COUNT; ++i) {
						Players[i].Update(&hInput);
					}
					
					LocationComparison();
					for(int i = 0; i < PLAYER_COUNT; ++i) {
						Players[i].Update(&hInput);
					}

					// Collision detection
					DetectCollisions();

					if(ControlChangeTimer.getElapsedTime() >= sf::seconds(CONTROL_CHANGE_DELAY)) {
						for(int i = 0; i < PLAYER_COUNT; ++i) {
							Players[i].ChangeControls(&hInput);
							ControlChangeTimer.restart();
						}
					}

					// Check win condition
					if(Players[0].dead) {
						winState = e_PLAYER_1_WIN;
						DeathTimer.restart();
					} else if(Players[1].dead) {
						winState = e_PLAYER_0_WIN;
						DeathTimer.restart();
					}
					break;
				}
			}
		case e_END:
			// ESCAPE - Close game
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Escape)) {
				window->close();
			}

			// RETURN - Start game again
			if(hInput.isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) {
				Initialise();
			}
			break;
	}
}

void Application::Render(sf::RenderWindow* window) {
	window->clear();

	switch(currentState) {
		case e_START:
			// Render background

			// Render foreground
		
			// Render UI
			window->draw(GameLogo);
			window->draw(TeamText);
			window->draw(StartScreenInfo1);

			break;
		case e_LOBBY:
			window->draw(Player1Txt);
			window->draw(Player2Txt);
			window->draw(Player1Status);
			window->draw(Player2Status);
			break;
		case e_INGAME:
			// Render background
			window->draw(Background.background);
			window->draw(Background.player1_healthbarB);
			window->draw(Background.player2_healthbarB);

			// Render foreground
			for(int i = 0; i < PLAYER_COUNT; ++i) {
				Players[i].animatedSprite.update(frameTime);
				window->draw(Players[i].animatedSprite);
			}
			
			// Render UI
			if(!gameStarted) {
				int count = 4 - StartTimer.getElapsedTime().asSeconds();
				CountdownText.setString(std::to_string(count));
				CountdownText.setPosition((RESOLUTION_X / 2) - (CountdownText.getGlobalBounds().width / 2), RESOLUTION_Y * 0.7);
				if(count != 0) {
					window->draw(CountdownText);
				}
			}

			Player1Health.setString(std::to_string(Players[0].health));
			Player1Health.setPosition(50, 25);
			Player2Health.setString(std::to_string(Players[1].health));
			Player2Health.setPosition((RESOLUTION_X - (Player2Health.getGlobalBounds().width)) - 50, 25);

			window->draw(Player1Health);
			window->draw(Player2Health);
			//window->draw(TimerText);
			//window->draw(Player1Text);
			//window->draw(Player2Text);

			break;
		case e_END:
			// Render background
			
			// Render foreground

			// Render UI
			window->draw(WinnerText);
			break;
	}

	window->display();
}

void Application::LocationComparison(){
	float x_a = Players[0].Position.x;
	float x_b = Players[1].Position.x;

	if (x_a > x_b && (Players[0].flipped == false && Players[1].flipped == true)){
		Players[0].flipped = true;
		Players[1].flipped = false;
		Players[0].needFlipped = true;
		Players[1].needFlipped = true;
	}
	else if (x_a < x_b && (Players[0].flipped == true && Players[1].flipped == false)){
		Players[0].flipped = false;
		Players[1].flipped = true;
		Players[0].needFlipped = true;
		Players[1].needFlipped = true;
	}
}

void Application::DetectCollisions() {
	if(GetCollision(Players[0], Players[1])) {		// If collision
		if(Players[0].isAttacking && Players[0].attackType != e_NO_ATTACK && (Players[0].AttackTimer.getElapsedTime() >= sf::seconds(0.35) && Players[0].AttackTimer.getElapsedTime() <= sf::seconds(0.4))) {
			// Damage enemy player
			Players[1].DepleteHealth(Players[0].attackType);
			// Knock player back
			if(Players[0].boundingBox.min.x < Players[1].boundingBox.min.x) {
				Players[1].Knockback(e_RIGHT, Players[1].attackType);
			} else {
				Players[1].Knockback(e_LEFT, Players[1].attackType);
			}
			Players[0].isAttacking = false;
		}
		if(Players[1].isAttacking && Players[1].attackType != e_NO_ATTACK && Players[1].AttackTimer.getElapsedTime() >= sf::seconds(0.35)) {
			// Damage enemy player
			Players[0].DepleteHealth(Players[1].attackType);
			// Knock player back
			if(Players[1].boundingBox.min.x < Players[0].boundingBox.min.x) {
				Players[0].Knockback(e_RIGHT, Players[0].attackType);
			} else {
				Players[0].Knockback(e_LEFT, Players[0].attackType);
			}
			Players[1].isAttacking = false;
		}
	}
}

bool Application::GetCollision(Player p1, Player p2) {
	if(p1.boundingBox.min.x > p2.boundingBox.max.x)	return false;
	if(p1.boundingBox.max.x < p2.boundingBox.min.x)	return false;
	if(p1.boundingBox.min.y > p2.boundingBox.max.y)	return false;
	if(p1.boundingBox.max.y < p2.boundingBox.min.y)	return false;

	return true;
}
