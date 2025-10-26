#include "Guard.h"

void Guard::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.5, 0.25), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(ENEMY_STAND_LEFT, 8);
	sprite->addKeyframe(ENEMY_STAND_LEFT, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(ENEMY_STAND_RIGHT, 8);
	sprite->addKeyframe(ENEMY_STAND_RIGHT, glm::vec2(0.0f, 0.75f));

	sprite->setAnimationSpeed(ENEMY_STAND_UP, 8);
	sprite->addKeyframe(ENEMY_STAND_UP, glm::vec2(0.0f, 0.25f));

	sprite->setAnimationSpeed(ENEMY_STAND_DOWN, 8);
	sprite->addKeyframe(ENEMY_STAND_DOWN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(ENEMY_MOVE_LEFT, 8);
	sprite->addKeyframe(ENEMY_MOVE_LEFT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(ENEMY_MOVE_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(ENEMY_MOVE_RIGHT, 8);
	sprite->addKeyframe(ENEMY_MOVE_RIGHT, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(ENEMY_MOVE_RIGHT, glm::vec2(0.5f, 0.75f));

	sprite->setAnimationSpeed(ENEMY_MOVE_UP, 8);
	sprite->addKeyframe(ENEMY_MOVE_UP, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(ENEMY_MOVE_UP, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(ENEMY_MOVE_DOWN, 8);
	sprite->addKeyframe(ENEMY_MOVE_DOWN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ENEMY_MOVE_DOWN, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(ENEMY_STAND_DOWN);
	tileMapDispl = tileMapPos;

	posEnemy = glm::vec2(0, 0);

	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));

	enemyState = PATROLLING;

}

void Guard::update(int deltaTime)
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


void Guard::attack(int deltaTime)
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
