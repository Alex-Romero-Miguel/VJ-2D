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

	health = 20;
	attackCooldown = 0;
	moveSpeed = 1.2f;

	timeBetweenShots = 0;

	shootingPoints = {
		glm::vec2(20, 20),
		glm::vec2(30, 20),
		glm::vec2(40, 20)
	};
}

void Boss::update(int deltaTime) {
	sprite->update(deltaTime);
	if (attackCooldown > 0) attackCooldown -= deltaTime;
	if (timeBetweenShots > 0) timeBetweenShots -= deltaTime;

	switch (state) {
	case IDLE:
		if (canSeePlayer()) {
			state = CHOOSING_TARGET;
			//Show dialogue
		}
		break;
	case CHOOSING_TARGET:
		targetPoint = findClosestShootingPoint();

		// 2. ACTUAR: Decide si moverse o atacar
		if (glm::distance(posEnemy, targetPoint) < 2.0f) {
			// Si ya está en el punto más cercano, empieza a atacar
			state = ATTACKING;
			burstShotsFired = 0; // Reinicia el contador de la ráfaga
		}
		else {
			// Si no, se mueve hacia allí
			state = MOVING;
		}
		break;

	case MOVING: {
		sprite->changeAnimation(BOSS_MOVE);

		// Si está lejos, se mueve hacia el objetivo
		if (glm::distance(posEnemy, targetPoint) > 2.0f) {
			glm::vec2 dir = glm::normalize(targetPoint - posEnemy);
			posEnemy += dir * moveSpeed;
		}
		else {
			// Si llegó, se detiene y se prepara para atacar
			posEnemy = targetPoint;
			state = ATTACKING;
			burstShotsFired = 0;
		}
		break;

		//// Encuentra el punto de disparo más cercano al jugador
		//glm::vec2 playerPos = player->getPosition();
		//glm::vec2 targetPoint = shootingPoints[0];
		//float minDist = glm::distance(playerPos, targetPoint);

		//for (auto& p : shootingPoints) {
		//	float dist = glm::distance(playerPos, p);
		//	if (dist < minDist) {
		//		minDist = dist;
		//		targetPoint = p;
		//	}
		//}

		//// Movimiento hacia ese punto
		//glm::vec2 dir = targetPoint - posEnemy;
		//float len = glm::length(dir);
		//if (len > 1.f) posEnemy += glm::normalize(dir) * moveSpeed;
		//else state = ATTACKING;

		//sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));

		//break;
	}

	case ATTACKING: { // Disparos
		//if (attackCooldown <= 0) {
		//	// Disparo hacia el jugador
		//	glm::vec2 playerPos = player->getPosition();
		//	glm::vec2 dir = glm::normalize(playerPos - posEnemy);

		//	/*Projectile p;
		//	p.init(posEnemy + glm::vec2(8, 8), dir * 3.0f, *shaderProgram);
		//	projectiles.push_back(p);*/

		//	attackCooldown = 600; // 0.6s cooldown
		//}
		//else attackCooldown -= deltaTime;

		//const int SHOTS_PER_BURST = 5;
		//if (burstShotsFired >= SHOTS_PER_BURST) {
		//	// Si terminó la ráfaga, entra en cooldown
		//	state = IDLE;
		//	attackCooldown = 2000; // Cooldown de 2 segundos antes de la próxima acción
		//}
		//break;

		stopMovingAnim();
		const int SHOTS_PER_BURST = 10;
		// Si ya disparó todas las balas de la ráfaga, entra en cooldown
		if (burstShotsFired >= SHOTS_PER_BURST) {
			state = COOLDOWN;
			attackCooldown = 1000; // Pausa de 1.5 segundos después de la ráfaga
		}
		break;
	}
	case COOLDOWN:
		stopMovingAnim();
		// Cuando termina la pausa, vuelve a evaluar la situación
		if (attackCooldown <= 0) {
			state = CHOOSING_TARGET;
		}
		break;
	case DEAD:
		//Animacion de muerte
		break;
	}

	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));
}

//Projectile Boss::shoot()
//{
//	Projectile p;
//	glm::vec2 dir = glm::normalize(glm::vec2(player->getPosition()) - posEnemy);
//
//	p.init(posEnemy + glm::vec2(8, 8), dir, *shaderProgram);
//	return p;
//}



//bool Boss::readyToShoot() const
//{
//	return state == ATTACKING && attackCooldown <= 0;
//}


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

