#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(STAND_UP, 8);
		sprite->addKeyframe(STAND_UP, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(STAND_DOWN, 8);
		sprite->addKeyframe(STAND_DOWN, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

		sprite->setAnimationSpeed(MOVE_UP, 8);
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_UP))
	{
		if(sprite->animation() != MOVE_UP)
			sprite->changeAnimation(MOVE_UP);
		posPlayer.y -= 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y += 2;
			sprite->changeAnimation(STAND_UP);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_DOWN))
	{
		if(sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPlayer.y += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= 2;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
		else if(sprite->animation() == MOVE_UP)
			sprite->changeAnimation(STAND_UP);
		else if(sprite->animation() == MOVE_DOWN)
			sprite->changeAnimation(STAND_DOWN);
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




