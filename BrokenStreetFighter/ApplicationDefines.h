// ApplicationDefines.h

#ifndef APPLICATIONDEFINES_H
#define APPLICATIONDEFINES_H

// INCLUDES
#include "InputHandler.h"

// STRUCTS
struct ControlLayout 
{
	Key* Up;
	Key* Left;
	Key* Right;
	Key* Weak;
	Key* Heavy;
};

struct AABB 
{
	sf::Vector2f max;
	sf::Vector2f min;
};


// DEFINES
#define	PLAYER_COUNT				2
#define CONTROL_LAYOUTS				2
#define GRAVITY						2
#define FRICTION					1
#define AIR_RESISTANCE				0.02
#define MOVE_SPEED					7.5
#define JUMP_SPEED					30
#define KNOCKBACK_WEAK_X			10
#define KNOCKBACK_WEAK_Y			-5
#define KNOCKBACK_HEAVY_X			15
#define KNOCKBACK_HEAVY_Y			-7.5

#define RESOLUTION_X				2340		
#define RESOLUTION_Y				1560
#define STAGE_MIN					0
#define STAGE_MAX					RESOLUTION_X - 400
#define FLOOR_Y						RESOLUTION_Y-400

#define PLAYER_1_START				STAGE_MIN + 200
#define PLAYER_2_START				STAGE_MAX - 200

#define START_COUNTDOWN				3
#define	CONTROL_CHANGE_DELAY		10
#define ATTACK_DELAY				1


// ENUMS
enum e_GameState 
{
	e_START = 0, e_INGAME, e_END
};

enum e_Direction 
{
	e_LEFT = 0, e_RIGHT
};

enum e_WinState 
{
	e_PLAYER_0_WIN = 0, e_PLAYER_1_WIN, e_ONGOING
};

enum e_AttackType 
{
	e_WEAK = 0, e_HEAVY, e_NO_ATTACK
};

// ETCS

#endif