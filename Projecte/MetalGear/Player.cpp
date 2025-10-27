#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/solid_snake_normal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));
		
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.375f, 0.0f));
		
	sprite->setAnimationSpeed(STAND_UP, 8);
	sprite->addKeyframe(STAND_UP, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(STAND_DOWN, 8);
	sprite->addKeyframe(STAND_DOWN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.125f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.125f));
		
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.125f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.875, 0.125f));

	sprite->setAnimationSpeed(MOVE_UP, 8);
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.25f, 0.125f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.375f, 0.125f));

	sprite->setAnimationSpeed(MOVE_DOWN, 8);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.0f, 0.125f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.125f, 0.125f));


	sprite->setAnimationSpeed(PUNCH_LEFT, 12);
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_RIGHT, 12);
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.375f, 0.0f));
	sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.375f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_UP, 12);
	sprite->addKeyframe(PUNCH_UP, glm::vec2(0.125f, 0.0f));
	sprite->addKeyframe(PUNCH_UP, glm::vec2(0.125f, 0.5f));

	sprite->setAnimationSpeed(PUNCH_DOWN, 12);
	sprite->addKeyframe(PUNCH_DOWN, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(PUNCH_DOWN, glm::vec2(0.0f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	facing = FACE_LEFT;

	health = STARTING_HEALTH;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	bool zDown = Game::instance().getKey(GLFW_KEY_Z);

	if (punching) {
		punchElapsedMs += deltaTime;
		if (punchElapsedMs >= punchDurationMs) {
			punchElapsedMs = 0;

			if (zDown) {
				switch (facing) {
				case FACE_LEFT:  sprite->changeAnimation(PUNCH_LEFT);  break;
				case FACE_RIGHT: sprite->changeAnimation(PUNCH_RIGHT); break;
				case FACE_UP:    sprite->changeAnimation(PUNCH_UP);    break;
				case FACE_DOWN:  sprite->changeAnimation(PUNCH_DOWN);  break;
				}
			}
			else {
				punching = false;
				switch (facing) {
				case FACE_LEFT:  sprite->changeAnimation(STAND_LEFT);  break;
				case FACE_RIGHT: sprite->changeAnimation(STAND_RIGHT); break;
				case FACE_UP:    sprite->changeAnimation(STAND_UP);    break;
				case FACE_DOWN:  sprite->changeAnimation(STAND_DOWN);  break;
				}
			}
		}

		zWasDown = zDown;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
			float(tileMapDispl.y + posPlayer.y)));
		return;
	}

	if (zDown && !zWasDown) {
		punching = true;
		punchElapsedMs = 0;
		switch (facing) {
			case FACE_LEFT:  sprite->changeAnimation(PUNCH_LEFT);  break;
			case FACE_RIGHT: sprite->changeAnimation(PUNCH_RIGHT); break;
			case FACE_UP:    sprite->changeAnimation(PUNCH_UP);    break;
			case FACE_DOWN:  sprite->changeAnimation(PUNCH_DOWN);  break;
		}

		zWasDown = zDown;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
			float(tileMapDispl.y + posPlayer.y)));
		return;
	}

	if(Game::instance().getKey(GLFW_KEY_LEFT) || Game::instance().getKey(GLFW_KEY_A))
	{
		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
			facing = FACE_LEFT;
		}
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(16, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT) || Game::instance().getKey(GLFW_KEY_D))
	{
		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
			facing = FACE_RIGHT;
		}
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(16, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_UP) || Game::instance().getKey(GLFW_KEY_W))
	{
		if (sprite->animation() != MOVE_UP) {
			sprite->changeAnimation(MOVE_UP);
			facing = FACE_UP;
		}
		posPlayer.y -= 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(16, 32)))
		{
			posPlayer.y += 2;
			sprite->changeAnimation(STAND_UP);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_DOWN) || Game::instance().getKey(GLFW_KEY_S))
	{
		if (sprite->animation() != MOVE_DOWN) {
			sprite->changeAnimation(MOVE_DOWN);
			facing = FACE_DOWN;
		}
		posPlayer.y += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(16, 32)))
		{
			posPlayer.y -= 2;
			sprite->changeAnimation(STAND_DOWN);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT )
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT )
			sprite->changeAnimation(STAND_RIGHT);
		else if(sprite->animation() == MOVE_UP )
			sprite->changeAnimation(STAND_UP);
		else if(sprite->animation() == MOVE_DOWN)
			sprite->changeAnimation(STAND_DOWN);
	}

	zWasDown = zDown;
	
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


void Player::takeDamage(int amount)
{
	health -= amount;
	if (health < 0) {
		health = 0; // Evita que la vida sea negativa
	}
}

bool Player::isDead() const
{
	return health <= 0;
}

int Player::getHealth() const
{
	return health;
}


