#include "Guard.h"

void Guard::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.5, 0.25), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(GUARD_STAND_LEFT, 8);
	sprite->addKeyframe(GUARD_STAND_LEFT, glm::vec2(0.0f, 0.5f));

	sprite->setAnimationSpeed(GUARD_STAND_RIGHT, 8);
	sprite->addKeyframe(GUARD_STAND_RIGHT, glm::vec2(0.0f, 0.75f));

	sprite->setAnimationSpeed(GUARD_STAND_UP, 8);
	sprite->addKeyframe(GUARD_STAND_UP, glm::vec2(0.0f, 0.25f));

	sprite->setAnimationSpeed(GUARD_STAND_DOWN, 8);
	sprite->addKeyframe(GUARD_STAND_DOWN, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(GUARD_MOVE_LEFT, 8);
	sprite->addKeyframe(GUARD_MOVE_LEFT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(GUARD_MOVE_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(GUARD_MOVE_RIGHT, 8);
	sprite->addKeyframe(GUARD_MOVE_RIGHT, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(GUARD_MOVE_RIGHT, glm::vec2(0.5f, 0.75f));

	sprite->setAnimationSpeed(GUARD_MOVE_UP, 8);
	sprite->addKeyframe(GUARD_MOVE_UP, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(GUARD_MOVE_UP, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(GUARD_MOVE_DOWN, 8);
	sprite->addKeyframe(GUARD_MOVE_DOWN, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(GUARD_MOVE_DOWN, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(GUARD_STAND_DOWN);
	tileMapDispl = tileMapPos;

	posEnemy = glm::vec2(0, 0);

	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));

	state = PATROLLING;

	health = 10;

}

void Guard::update(int deltaTime)
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
		verticalVel += 0.0012f * deltaTime; // gravedad
		verticalPos += verticalVel * deltaTime;

		if (verticalPos > 0.f) {
			verticalPos = 0.f;
			knockUp = false;
		}

		sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
			tileMapDispl.y + posEnemy.y + verticalPos));
		return; // se detiene el resto de la IA mientras cae
	}

	if (isHurt) {
		hurtTimer -= deltaTime;
		if (hurtTimer <= 0) isHurt = false;
	}

	switch (state)
	{
	case PATROLLING:
		patrol();
		if (canSeePlayer()) {
			state = CHASING;
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



void Guard::stopMovingAnim() {
	switch (facing) {
	case FACE_LEFT:  sprite->changeAnimation(GUARD_STAND_LEFT);  break;
	case FACE_RIGHT: sprite->changeAnimation(GUARD_STAND_RIGHT); break;
	case FACE_UP:    sprite->changeAnimation(GUARD_STAND_UP);    break;
	case FACE_DOWN:  sprite->changeAnimation(GUARD_STAND_DOWN);  break;
	}
}


void Guard::changeDirAnim(glm::vec2 dir) {
	GuardAnim newAnim;
	if (fabs(dir.x) > fabs(dir.y)) {
		if (dir.x > 0) { newAnim = GUARD_MOVE_RIGHT; facing = FACE_RIGHT; }
		else { newAnim = GUARD_MOVE_LEFT; facing = FACE_LEFT; }
	}
	else {
		if (dir.y > 0) { newAnim = GUARD_MOVE_DOWN; facing = FACE_DOWN; }
		else { newAnim = GUARD_MOVE_UP; facing = FACE_UP; }
	}

	if (sprite->animation() != newAnim)
		sprite->changeAnimation(newAnim);
}

bool Guard::attack(int deltaTime)
{
	return false;
}
