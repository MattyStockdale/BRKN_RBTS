// GameObj.cpp
#include "GameObj.h"

GameObj::GameObj()
{
	
}

GameObj::~GameObj()
{

}

sf::Texture GameObj::SetTexture(sf::String fileToLoad)
{
	std::string errorString = fileToLoad;
	if (!texture.loadFromFile(fileToLoad))
	{
		std::cout << "Failed to Load Spritesheet at " << errorString << std::endl;
	}
	return texture;
}

void GameObj::SetUpAnimatedSprite()
{
	animatedSprite.setFrameTime(sf::seconds(0.1));
	animatedSprite.setScale(15, 15);
	animatedSprite.pause();
	animatedSprite.setLooped(true);
	animatedSprite.setPosition(Position);
}

void GameObj::SetAnimationFrames(Animation* animation, sf::Vector2f position, int frames, int xMaxValue, int spriteWidth, int spriteHeight)
{
	int xStartingPosition = position.x;
	for (int frame = 0; frame < frames; frame++)
	{
		animation->addFrame(sf::IntRect(position.x, position.y, spriteWidth, spriteHeight));

		if (position.x >= xMaxValue)
		{
			position.x = xStartingPosition;
			position.y += spriteHeight;
		}
		else
		{
			position.x += spriteWidth;
		}
	}
}

void GameObj::AddNewAnimation(sf::String Id, sf::Vector2f position, int frames, int xMaxValue, int spriteWidth, int spriteHeight)
{
	Animation anim;
	anim.setSpriteSheet(texture);
	SetAnimationFrames(&anim,position, frames, xMaxValue, spriteWidth, spriteHeight);
	animations[Id]=anim;
}