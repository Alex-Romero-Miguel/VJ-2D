#include "Dog.h"

void Dog::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/dog.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.2), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(DOG_IDLE, 8);
	sprite->addKeyframe(DOG_IDLE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(DOG_IDLE, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(DOG_MOVE_DOWN, 8);
	sprite->addKeyframe(DOG_MOVE_DOWN, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(DOG_MOVE_DOWN, glm::vec2(0.5f, 0.2f));

	sprite->setAnimationSpeed(DOG_MOVE_UP, 8);
	sprite->addKeyframe(DOG_MOVE_UP, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(DOG_MOVE_UP, glm::vec2(0.5f, 0.4f));

	sprite->setAnimationSpeed(DOG_MOVE_LEFT, 8);
	sprite->addKeyframe(DOG_MOVE_LEFT, glm::vec2(0.0f, 0.6f));
	sprite->addKeyframe(DOG_MOVE_LEFT, glm::vec2(0.5f, 0.6f));


	sprite->setAnimationSpeed(DOG_MOVE_RIGHT, 8);
	sprite->addKeyframe(DOG_MOVE_RIGHT, glm::vec2(0.0f, 0.8f));
	sprite->addKeyframe(DOG_MOVE_RIGHT, glm::vec2(0.5f, 0.8f));

	tileMapDispl = tileMapPos;
	posEnemy = glm::vec2(0, 0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

	sprite->changeAnimation(DOG_IDLE);
	
	state = PATROLLING;

	facing = FACE_DOWN;
	health = 2;
}

void Dog::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (dead) {
		deadTimer -= deltaTime;
		if (deadTimer <= 0) {
			toRemove = true; // marcar para borrar
		}
		return;
	}

	if (knockUp) {
		verticalVel += 0.0012f * deltaTime; 
		verticalPos += verticalVel * deltaTime;

		if (verticalPos > 0.f) {
			verticalPos = 0.f;
			knockUp = false;
		}

		sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
			tileMapDispl.y + posEnemy.y + verticalPos));
		return; 
	}


	if (isHurt) {
		hurtTimer -= deltaTime;
		if (hurtTimer <= 0) isHurt = false;
	}

	switch (state)
	{
	case PATROLLING:
		sprite->changeAnimation(DOG_IDLE);
		if (canSeePlayer()) {
			state = CHASING;
			updatePathToPlayer();
		}
		break;
	case CHASING:
		chase(deltaTime);
		if (glm::distance(posEnemy, glm::vec2(player->getPosition())) < 32.f) {
			state = ATTACKING;
		}
		break;
	case ATTACKING: 
		attack(deltaTime);
		break;
	case RETURNING:
		break;

	case DEAD: 

		break;
	}
	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));
}

void Dog::changeDirAnim(glm::vec2 dir) {
	DogAnim newAnim;
	if (fabs(dir.x) > fabs(dir.y)) {
		if (dir.x > 0) { newAnim = DOG_MOVE_RIGHT; facing = FACE_RIGHT; }
		else { newAnim = DOG_MOVE_LEFT; facing = FACE_LEFT; }
	}
	else {
		if (dir.y > 0) { newAnim = DOG_MOVE_DOWN; facing = FACE_DOWN; }
		else { newAnim = DOG_MOVE_UP; facing = FACE_UP; }
	}

	if (sprite->animation() != newAnim)
		sprite->changeAnimation(newAnim);
}

void Dog::stopMovingAnim() {
}



bool Dog::attack(int deltaTime)
{
	if (!player) return false;

	glm::vec2 playerPos = player->getPosition();
	glm::vec2 diff = playerPos - posEnemy;
	float distance = glm::length(diff);

	if (distance > biteRange) {
		state = CHASING;
		return false;
	}

	if (attackCooldown > 0)
		attackCooldown -= deltaTime;

	if (attackCooldown <= 0) {
		attackCooldown = 1000;
		// animación y empuje
		switch (facing)
		{
		case FACE_LEFT:  sprite->changeAnimation(DOG_MOVE_LEFT); break;
		case FACE_RIGHT: sprite->changeAnimation(DOG_MOVE_RIGHT); break;
		case FACE_UP:    sprite->changeAnimation(DOG_MOVE_UP); break;
		case FACE_DOWN:  sprite->changeAnimation(DOG_MOVE_DOWN); break;
		}

		posEnemy += glm::normalize(diff) * 4.f;
		return true; // indica a la escena que el ataque fue exitoso
	}

	return false;
}
