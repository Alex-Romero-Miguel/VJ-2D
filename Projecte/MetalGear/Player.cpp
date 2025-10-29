#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

Player::Player()
{
	sprite = NULL;
}

Player::~Player()
{
	if(sprite != NULL)
		delete sprite;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/solid_snake_normal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(13);
	
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

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.0f, 0.75f));
	sprite->addKeyframe(DEAD, glm::vec2(0.75f, 0.75f));

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	posPlayer = glm::ivec2(0, 0);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	facing = FACE_RIGHT;

	health = STARTING_HEALTH;
	this->shaderProgram = &shaderProgram;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (dead) sprite->changeAnimation(DEAD);

	if (Game::instance().getKey(GLFW_KEY_C)) {
		// canviar item equipat
	}

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

	if (isHurt) {
		hurtTimer -= deltaTime;
		hurtBlinkTime += deltaTime * 0.01f; 

		if (hurtTimer <= 0) {
			isHurt = false;
			hurtTimer = 0;
			hurtBlinkTime = 0.f;
		}
	}

	
	glm::vec2 nextPos = posPlayer; // Empezamos con la posición actual

	if (Game::instance().getKey(GLFW_KEY_LEFT) || Game::instance().getKey(GLFW_KEY_A))
	{
		nextPos.x -= 2; // Calculamos la posible nueva posición
		if (sprite->animation() != MOVE_LEFT) {
			sprite->changeAnimation(MOVE_LEFT);
			facing = FACE_LEFT;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT) || Game::instance().getKey(GLFW_KEY_D))
	{
		nextPos.x += 2;
		if (sprite->animation() != MOVE_RIGHT) {
			sprite->changeAnimation(MOVE_RIGHT);
			facing = FACE_RIGHT;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_UP) || Game::instance().getKey(GLFW_KEY_W))
	{
		nextPos.y -= 2;
		if (sprite->animation() != MOVE_UP) {
			sprite->changeAnimation(MOVE_UP);
			facing = FACE_UP;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN) || Game::instance().getKey(GLFW_KEY_S))
	{
		nextPos.y += 2;
		if (sprite->animation() != MOVE_DOWN) {
			sprite->changeAnimation(MOVE_DOWN);
			facing = FACE_DOWN;
		}
	}
	else
	{
		// Si no se pulsa ninguna tecla, cambia a la animación de estar quieto
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		else if (sprite->animation() == MOVE_UP) sprite->changeAnimation(STAND_UP);
		else if (sprite->animation() == MOVE_DOWN) sprite->changeAnimation(STAND_DOWN);
	}

	// Comprobá la colisión en la posición de destino ANTES de mover al jugador
	glm::ivec2 realNextPos = glm::ivec2(nextPos.x + colliderOffset.x, nextPos.y + colliderOffset.y);
	if (nextPos.x != posPlayer.x) {
		if (nextPos.x > posPlayer.x) { // Se mueve a la derecha
			if (!map->collisionMoveRight(realNextPos, colliderSize)) {
				posPlayer.x = nextPos.x;
			}
		}
		else { // Se mueve a la izquierda
			if (!map->collisionMoveLeft(realNextPos, colliderSize)) {
				posPlayer.x = nextPos.x;
			}
		}
	}

	if (nextPos.y != posPlayer.y) {
		if (nextPos.y > posPlayer.y) { // Se mueve hacia abajo
			if (!map->collisionMoveDown(realNextPos, colliderSize)) {
				posPlayer.y = nextPos.y;
			}
		}
		else { // Se mueve hacia arriba
			if (!map->collisionMoveUp(realNextPos, colliderSize)) {
				posPlayer.y = nextPos.y;
			}
		}
	}

	zWasDown = zDown;

	//std::cout << "Health: " << health << std::endl;
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	//std::cout << posPlayer.x/8.f << " " << posPlayer.y/8.f << std::endl;
}

void Player::render()
{
	shaderProgram->use();

	if (isHurt) {
		int cycle = int(hurtTimer / 75.f) % 2; 

		if (cycle == 0) {
			shaderProgram->setUniform4f("tintColor", 1.0f, 0.0f, 1.0f, 1.0f); // Rojo
		}
		else {
			shaderProgram->setUniform4f("tintColor", 1.0f, 1.0f, 1.0f, 1.0f); // Normal
		}
	}
	else {
		shaderProgram->setUniform4f("tintColor", 1.0f, 1.0f, 1.0f, 1.0f);
	}
	sprite->render();
	shaderProgram->setUniform4f("tintColor", 1.0f, 1.0f, 1.0f, 1.0f);
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
	
	if (isHurt) return; 
	health -= amount;
	if (health < 0) {
		health = 0; // Evita que la vida sea negativa
		dead = true;
	}
	isHurt = true;
	hurtTimer = 500.f;
}

void Player::heal(int amount)
{
	health += amount;
	if (health > STARTING_HEALTH)
		health = STARTING_HEALTH;
}

void Player::pickUpItem(Item *item)
{
	inventory.push_back(item);
}

void Player::useItem()
{
	inventory[current_item]->use(this);
}

void Player::changeItem()
{
	current_item += 1;
	int max = inventory.size();
	if (current_item >= max) 
		current_item = 0;
}

void Player::consumeItem() 
{
	inventory.erase(inventory.begin() + current_item);
}

bool Player::isDead() const
{
	return health <= 0;
}

int Player::getHealth() const
{
	return health;
}

float Player::getHealthPercentage() const
{
	return float(health) / STARTING_HEALTH;
}


glm::ivec4 Player::getPunchHitbox() const {
	if (!punching) return glm::ivec4(0, 0, 0, 0);

	glm::ivec2 pos = posPlayer;
	glm::ivec2 size = getSize();
	int range = 10; // distancia del golpe

	switch (facing) {
	case FACE_LEFT:  return glm::ivec4(pos.x - range, pos.y, range, size.y);
	case FACE_RIGHT: return glm::ivec4(pos.x + size.x, pos.y, range, size.y);
	case FACE_UP:    return glm::ivec4(pos.x, pos.y - range, size.x, range);
	case FACE_DOWN:  return glm::ivec4(pos.x, pos.y + size.y, size.x, range);
	}
	return glm::ivec4(0, 0, 0, 0);
}