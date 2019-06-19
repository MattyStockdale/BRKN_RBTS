// Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include "GameObj.h"
#include "ApplicationDefines.h"

class Player : public GameObj 
{
public:
	Player();
	~Player();
	
	int time;

	bool Initialise(InputHandler* hInput, int i, sf::Vector2f p, bool startFlipped, bool isRestart = false);
	void Update(InputHandler* hInput);
	void ChangeControls(InputHandler* hInput);
	void Knockback(e_Direction d, e_AttackType at);
	void DepleteHealth(e_AttackType at);
	void Dead();
	void SetVariables(sf::Vector2f p, bool startFlipped);

	bool isPunchingW;
	bool isPunchingH;

	AABB boundingBox;
	e_AttackType attackType;
	sf::Clock AttackTimer;
	bool isAttacking;
	bool dead;
	int health;

protected:


private:
	void InitialiseControls(InputHandler* hInput, int i);

	void HandleInput(InputHandler* hInput);
	void RepositionPlayer();
	void UpdateAABB();
	void Move(e_Direction d);
	void Jump();
	void Attack(e_AttackType at);

	ControlLayout controls[CONTROL_LAYOUTS];
	ControlLayout* currentControls;
	int layout;

#define SPRITE_HEIGHT 52;
#define SPRITE_WIDTH 52;
#define X_MAX 260;

	const int spriteHeight = 52;
	const int spriteWidth = 52;
	const int spritesheetXMax = 260;


	sf::Texture normalTexture;
	sf::Texture invertedTexture;


	
	sf::Vector2f velocity;
	float Width;
	float Height;

	bool inAir;
	bool knockback;
};

#endif