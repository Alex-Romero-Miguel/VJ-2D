#include "Dog.h"


void Dog::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/dog.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.2), &spritesheet, &shaderProgram);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

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
	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,tileMapDispl.y + posEnemy.y));

	posEnemy = glm::vec2(0, 0);
	enemyState = PATROLLING;
	facing = FACE_DOWN;
}

void Dog::update(int deltaTime)
{
	sprite->update(deltaTime);

	switch (enemyState)
	{
	case PATROLLING:
		patrol();
		if (canSeePlayer()) {
			enemyState = CHASING;
			updatePathToPlayer();
		}
		break;
	case CHASING:
		chase(deltaTime);
		break;
	case RETURNING:
		patrol();
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

void Dog::attack(int deltaTime)
{
	if (!player) return;

	//shootCooldown -= deltaTime;
	//if (shootCooldown <= 0) {
	//	shootCooldown = shootRate;

	//	// --- Crear proyectil ---
	//	glm::vec2 dir = glm::normalize(player->getPosition() - posEnemy);
	//	glm::vec2 bulletPos = posEnemy + dir * 16.f; // sale desde frente del enemigo

	//	isShooting = true;
	//	//sprite->changeAnimation(ENEMY_ATTACK);
	//}
	//else {
	//	isShooting = false;
	//}
}
