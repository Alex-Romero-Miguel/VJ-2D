#include "Boss.h"
#include "Scene.h"

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) 
{
	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(BOSS_STAND, 8);
	sprite->addKeyframe(BOSS_STAND, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(BOSS_MOVE, 8);
	sprite->addKeyframe(BOSS_MOVE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(BOSS_MOVE, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(BOSS_SHOOT, 8);
	sprite->addKeyframe(BOSS_SHOOT, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(BOSS_SHOOT, glm::vec2(0.5f, 0.0f));

	sprite->changeAnimation(BOSS_STAND);
	
	tileMapDispl = tileMapPos;
	posEnemy = glm::vec2(30, 20);

	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));

	this->shaderProgram = &shaderProgram;

	state = IDLE;
	facing = FACE_DOWN;

	health = 15;
	attackCooldown = 0;
	moveSpeed = 1.2f;

	timeBetweenShots = 0;

	shootingPoints = {
		glm::vec2(26, 30),
		glm::vec2(116, 30),
		glm::vec2(218, 30)
	};
	//currentTargetIndex = 0;
}

void Boss::update(int deltaTime) {

    sprite->update(deltaTime);
    if (attackCooldown > 0) attackCooldown -= deltaTime;
    if (timeBetweenShots > 0) timeBetweenShots -= deltaTime;

    if (isHurt) {
        hurtTimer -= deltaTime;
        if (hurtTimer <= 0) {
            isHurt = false;
            hurtTimer = 0;
        }
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
        return; // se detiene el resto de la IA mientras cae
    }


    switch (state) {
    case IDLE:
        state = CHOOSING_TARGET;
        break;

    case CHOOSING_TARGET:
        targetPoint = shootingPoints[currentTargetIndex];
        state = MOVING;
        sprite->changeAnimation(BOSS_MOVE);
        break;

    case MOVING: {
        glm::vec2 diff = targetPoint - posEnemy;
        float len = glm::length(diff);
        if (len > 0.01f)
            posEnemy += (diff / len) * moveSpeed * (deltaTime / 16.f);

        // Cuando llega al punto, se detiene para atacar
        if (glm::distance(posEnemy, targetPoint) < 2.0f) {
            posEnemy = targetPoint;
            state = ATTACKING;
            burstShotsFired = 0;
            sprite->changeAnimation(BOSS_SHOOT);
        }
        break;
    }

    case ATTACKING: {
        // Dispara una ráfaga de balas
        const int SHOTS_PER_BURST = 5;
        const int TIME_BETWEEN_SHOTS = 300; // ms entre disparos

        if (burstShotsFired < SHOTS_PER_BURST && attackCooldown <= 0) {
            // Aquí creas el proyectil
            glm::vec2 dir = glm::normalize(glm::vec2(player->getPosition()) - posEnemy);
            // projectileManager->spawn(posEnemy + glm::vec2(8,8), dir * 4.f);
            burstShotsFired++;
            attackCooldown = TIME_BETWEEN_SHOTS;
        }

        // Cuando termina la ráfaga, pausa antes de moverse al siguiente punto
        if (burstShotsFired >= SHOTS_PER_BURST && attackCooldown <= 0) {
            state = COOLDOWN;
            attackCooldown = 800; // pausa breve antes de continuar
            sprite->changeAnimation(BOSS_STAND);
        }
        break;
    }

    case COOLDOWN:
        if (attackCooldown > 0)
            attackCooldown -= deltaTime;
        else {
            // Cambiar al siguiente punto de patrulla
            currentTargetIndex = (currentTargetIndex + 1) % shootingPoints.size();
            state = CHOOSING_TARGET;
        }
        break;

    case DEAD:
        break;
    }

    sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
        tileMapDispl.y + posEnemy.y));
}



bool Boss::attack(int deltaTime)
{
	if (state == ATTACKING && attackCooldown <= 0) {
		attackCooldown = 600; // Cooldown de 0.6 segundos entre disparos
		return true; // Devuelve true para indicarle a la Scene que cree una bala
	}
	return false;
}


glm::vec2 Boss::findClosestShootingPoint()
{
	if (!player) return shootingPoints[0];

	glm::vec2 playerPos = player->getPosition();
	glm::vec2 closestPoint = shootingPoints[0];
	float minDist = glm::distance(playerPos, closestPoint);

	for (size_t i = 1; i < shootingPoints.size(); ++i) {
		float dist = glm::distance(playerPos, shootingPoints[i]);
		if (dist < minDist) {
			minDist = dist;
			closestPoint = shootingPoints[i];
		}
	}
	return closestPoint;
}


