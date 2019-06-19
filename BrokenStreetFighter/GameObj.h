// GameObj.h

#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <vector>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "AnimatedSprite.hpp"
#include <iostream>

class GameObj 
{
public:
	GameObj();
	virtual ~GameObj();
	sf::Sprite sprite;

	sf::Texture SetTexture(sf::String fileToLoad);
	sf::Texture texture;
	sf::Vector2f Position;

	AnimatedSprite animatedSprite;

	void SetUpAnimatedSprite();
	bool flipped;

protected:
	Animation* currentAnimation;
	std::map<sf::String, Animation> animations;

	void AddNewAnimation(sf::String Id, sf::Vector2f position, int frames, int xMaxValue, int spriteWidth, int spriteHeight);
	void SetAnimationFrames(Animation* animation, sf::Vector2f position, int frames, int xMaxValue, int spriteWidth, int spriteHeight);
};

#endif