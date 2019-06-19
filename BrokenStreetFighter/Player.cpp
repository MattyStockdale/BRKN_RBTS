// Player.cpp
#include "Player.h"

Player::Player() 
{
}

Player::~Player() 
{
}

bool Player::Initialise(InputHandler* hInput, int i, sf::Vector2f p, bool startFlipped, bool isRestart)
{
	// setup Animations 
	SetVariables(p, startFlipped);

	normalTexture = SetTexture("bin/AGDSjam_spritesheet.png");
	invertedTexture = SetTexture("bin/AGDSjam_spritesheet_flipped.png");

	if (!isRestart)
	{
		AddNewAnimation("Idle", sf::Vector2f(0, 0), 6, spritesheetXMax, spriteWidth, spriteHeight);
		AddNewAnimation("Forwards", sf::Vector2f(0, 52), 4, spritesheetXMax, spriteWidth, spriteHeight);
		AddNewAnimation("Backwards", sf::Vector2f(0, 104), 4, spritesheetXMax, spriteWidth, spriteHeight);
		AddNewAnimation("HardPunch", sf::Vector2f(0, 156), 6, spritesheetXMax, spriteWidth, spriteHeight);
		AddNewAnimation("LightPunch", sf::Vector2f(0, 208), 5, spritesheetXMax, spriteWidth, spriteHeight);
		AddNewAnimation("Death", sf::Vector2f(0, 260), 15, spritesheetXMax, spriteWidth, spriteHeight);
		currentAnimation = &animations.at("Idle");
	}

	InitialiseControls(hInput, i);
	Update(hInput);
	currentControls = &controls[0];
	return true;
}

void Player::SetVariables(sf::Vector2f p, bool startFlipped)
{
	SetUpAnimatedSprite();

	Position = p;
	Width = 52 * 8;
	Height = 52 * 8;
	velocity = sf::Vector2f(0.0f, 0.0f);

	health = 100;
	dead = false;
	inAir = false;
	isPunchingW = false;
	isPunchingH = false;

	isAttacking = false;
	attackType = e_NO_ATTACK;
	AttackTimer.restart();
	knockback = false;
	flipped = startFlipped;

	layout = 0;
	time = 0;
}

void Player::InitialiseControls(InputHandler* hInput, int i) 
{
	switch(i) 
	{
		case 0:
			controls[0].Left = hInput->getKey(e_KEYBOARD, sf::Keyboard::A);
			controls[0].Right = hInput->getKey(e_KEYBOARD, sf::Keyboard::D);
			controls[0].Up = hInput->getKey(e_KEYBOARD, sf::Keyboard::W);
			controls[0].Weak = hInput->getKey(e_KEYBOARD, sf::Keyboard::LShift);
			controls[0].Heavy = hInput->getKey(e_KEYBOARD, sf::Keyboard::LControl);

			controls[1].Left = hInput->getKey(e_KEYBOARD, sf::Keyboard::D);
			controls[1].Right = hInput->getKey(e_KEYBOARD, sf::Keyboard::A);
			controls[1].Up = hInput->getKey(e_KEYBOARD, sf::Keyboard::S);
			controls[1].Weak = hInput->getKey(e_KEYBOARD, sf::Keyboard::LControl);
			controls[1].Heavy = hInput->getKey(e_KEYBOARD, sf::Keyboard::LShift);
			currentControls = &controls[0];
			break;
		case 1:
			controls[0].Left = hInput->getKey(e_KEYBOARD, sf::Keyboard::Left);
			controls[0].Right = hInput->getKey(e_KEYBOARD, sf::Keyboard::Right);
			controls[0].Up = hInput->getKey(e_KEYBOARD, sf::Keyboard::Up);
			controls[0].Weak = hInput->getKey(e_KEYBOARD, sf::Keyboard::RShift);
			controls[0].Heavy = hInput->getKey(e_KEYBOARD, sf::Keyboard::RControl);

			controls[1].Left = hInput->getKey(e_KEYBOARD, sf::Keyboard::Right);
			controls[1].Right = hInput->getKey(e_KEYBOARD, sf::Keyboard::Left);
			controls[1].Up = hInput->getKey(e_KEYBOARD, sf::Keyboard::Down);
			controls[1].Weak = hInput->getKey(e_KEYBOARD, sf::Keyboard::RControl);
			controls[1].Heavy = hInput->getKey(e_KEYBOARD, sf::Keyboard::RShift);
			currentControls = &controls[0];
			break;
	}
}

void Player::Update(InputHandler* hInput) 
{
	if (animatedSprite.m_currentFrame >= 5 && isPunchingH == true)
	{
		isPunchingH = false;
	}
	if (animatedSprite.m_currentFrame >= 4 && isPunchingW == true)
	{
		isPunchingW = false;
	}
	
	if (flipped == true )
	{
		texture = invertedTexture;
	}
	else if (flipped == false )
	{
		texture = normalTexture;
	}

	if(AttackTimer.getElapsedTime() >= sf::seconds(ATTACK_DELAY)) 
	{
		attackType = e_NO_ATTACK;
	}

	HandleInput(hInput);
	RepositionPlayer();
	animatedSprite.play(*currentAnimation);
}

void Player::ChangeControls(InputHandler* hInput) 
{
	++layout;
	if(layout > (CONTROL_LAYOUTS - 1)) 
	{
		layout = 0;
	}
	currentControls = &controls[layout];
}

void Player::Knockback(e_Direction d, e_AttackType at) 
{
	sf::Vector2f strength;
	if(at == e_WEAK) 
	{
		strength = sf::Vector2f(KNOCKBACK_WEAK_X, KNOCKBACK_WEAK_Y);
	} 
	else 
	{
		strength = sf::Vector2f(KNOCKBACK_HEAVY_X, KNOCKBACK_HEAVY_Y);
	}

	if(d == e_LEFT)
	{
		strength.x = -strength.x;
	}

	velocity = strength;
	knockback = true;
}

void Player::DepleteHealth(e_AttackType at) 
{
	switch(at) 
	{
		case e_WEAK:
			health -= 5;
			break;
		case e_HEAVY:
			health -= 10;
			break;
	}
	if(health <= 0) 
	{
		dead = true;
		health = 0;
	}
}


void Player::HandleInput(InputHandler* hInput)
{
	if(AttackTimer.getElapsedTime() >= sf::seconds(ATTACK_DELAY)) 
	{
		isAttacking = false;
	}
	if(hInput->isKeyPressed(e_KEYBOARD, sf::Keyboard::Return)) 
	{
		ChangeControls(hInput);
	}
	if(!knockback) 
	{
		// JUMP
		if(currentControls->Up->pressed && currentControls->Up->changed)
		{
			Jump();
		}
		// HEAVY ATTACK
		if (currentControls->Heavy->pressed && currentControls->Heavy->changed) 
		{
			Attack(e_HEAVY);
			currentAnimation = &animations.at("HardPunch");
			isPunchingH = true;
		} 
		// WEAK ATTACK
		else if(currentControls->Weak->pressed && currentControls->Weak->changed) 
		{
			Attack(e_WEAK);
			currentAnimation = &animations.at("LightPunch");
			isPunchingW = true;
		} 
		// MOVE LEFT
		else if(currentControls->Left->pressed)
		{
			Move(e_LEFT);
			if (flipped == true && (isPunchingW == false || isPunchingH == false))
			{
				currentAnimation = &animations.at("Forwards");

			}
			else currentAnimation = &animations.at("Backwards");

		}
		// MOVE RIGHT
		else if(currentControls->Right->pressed) 
		{
			Move(e_RIGHT);
			if (flipped == true && (isPunchingW == false || isPunchingH == false))
			{
				currentAnimation = &animations.at("Backwards");
			}
			else currentAnimation = &animations.at("Forwards");
		}
		else if (isPunchingW == false && isPunchingH == false)
		{
			currentAnimation = &animations.at("Idle");
		}
	}
}

void Player::RepositionPlayer() 
{
	// POSITION X
	Position.x += velocity.x;
	if(inAir) 
	{			
		// If currently in air
		if(velocity.x > 0) 
		{	
			// If moving right
			velocity.x -= AIR_RESISTANCE;
			if(velocity.x < 0)
			{	
				// If velocity has flipped
				velocity.x = 0;
			}
		} 
		else 
		{				
			// If moving left
			velocity.x += AIR_RESISTANCE;
			if(velocity.x > 0) 
			{	
				// If velocity has flipped
				velocity.x = 0;
			}
		}
	} 
	else 
	{
		if(velocity.x > 0)
		{	
			// If moving right
			velocity.x -= FRICTION;
			if(velocity.x < 0) 
			{	
				// If velocity has flipped
				velocity.x = 0;
			}
		} 
		else 
		{				
			// If moving left
			velocity.x += FRICTION;
			if(velocity.x > 0) 
			{	
				// If velocity has flipped
				velocity.x = 0;
			}
		}
	}

	if(Position.x - (Width / 2) < STAGE_MIN)
	{
		Position.x = STAGE_MIN + (Width / 2);
		velocity.x = 0;
	}if(Position.x + (Width / 2) > STAGE_MAX) 
	{
		Position.x = STAGE_MAX - (Width / 2);
		velocity.x = 0;
	}

	// POSITION Y
	Position.y += velocity.y;
	velocity.y += GRAVITY;
	
	if(Position.y >= FLOOR_Y) 
	{
		Position.y = FLOOR_Y;
		inAir = false;
		knockback = false;
	} 
	else 
	{
		inAir = true;
	}

	// Set sprite position
	UpdateAABB();
	animatedSprite.setPosition(boundingBox.min.x, boundingBox.min.y);
}

void Player::UpdateAABB() 
{
	boundingBox.max.x = Position.x + (Width / 2);
	boundingBox.max.y = Position.y;
	
	boundingBox.min.x = Position.x - (Width / 2);
	boundingBox.min.y = Position.y - Height;
}

void Player::Move(e_Direction d) 
{
	if(!inAir)
	{
		if(d == e_LEFT) 
		{
			velocity.x = -MOVE_SPEED;
		} 
		else 
		{
			velocity.x = MOVE_SPEED;
		}
	}
}

void Player::Jump() 
{
	if(!inAir) 
	{
		inAir = true;
		velocity.y = -JUMP_SPEED;
	}
}

void Player::Attack(e_AttackType at) 
{
	if(AttackTimer.getElapsedTime() >= sf::seconds(ATTACK_DELAY) && !isAttacking)
	{
		isAttacking = true;
		AttackTimer.restart();
		if(at == e_WEAK) 
		{
			attackType = e_WEAK;
		} 
		else 
		{
			attackType = e_HEAVY;
		}
	}
}

void Player::Dead()
{
	animatedSprite.setLooped(false);
	currentAnimation = &animations.at("Death");;
	animatedSprite.play(*currentAnimation);
}