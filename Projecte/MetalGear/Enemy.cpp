#include "Enemy.h"
#include "Game.h"
#include <iostream>

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
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

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);

	switch (enemyState)
	{
	case PATROLLING:
		patrol();
		if (canSeePlayer()) {
			enemyState = CHASING;
			//updatePathToPlayer();
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

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
	pathfinder = std::make_unique<AStar>(map);
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,
		tileMapDispl.y + posEnemy.y));
}

void Enemy::setPlayer(Player* p)
{
	player = p;
}

bool Enemy::canSeePlayer()
{
	if (!player || !map) return false;

	glm::vec2 enemyCenter = glm::vec2(posEnemy.x + getSize().x / 2, posEnemy.y + getSize().y / 2);
	glm::vec2 playerCenter = glm::vec2(player->getPosition().x + player->getSize().x / 2,
		player->getPosition().y + player->getSize().y / 2);
	glm::vec2 diff = playerCenter - enemyCenter;

	float distance = glm::length(diff);
	if (distance > visionRange) return false;

	const float visionWidth = 16.f;

	bool inSight = false;
	switch (facing)
	{
	case FACE_LEFT:  if (diff.x < 0 && fabs(diff.y) < visionWidth) inSight = true; break;
	case FACE_RIGHT: if (diff.x > 0 && fabs(diff.y) < visionWidth) inSight = true; break;
	case FACE_UP:    if (diff.y < 0 && fabs(diff.x) < visionWidth) inSight = true; break;
	case FACE_DOWN:  if (diff.y > 0 && fabs(diff.x) < visionWidth) inSight = true; break;
	}
	if (!inSight) return false;

	glm::vec2 dir = glm::normalize(diff);
	const float step = 8.f;

	return true;
}

void Enemy::patrol()
{
	switch (facing) {
	case FACE_LEFT:  sprite->changeAnimation(ENEMY_STAND_LEFT);  break;
	case FACE_RIGHT: sprite->changeAnimation(ENEMY_STAND_RIGHT); break;
	case FACE_UP:    sprite->changeAnimation(ENEMY_STAND_UP);    break;
	case FACE_DOWN:  sprite->changeAnimation(ENEMY_STAND_DOWN);  break;
	}
}

void Enemy::chase(int deltaTime)
{
	if (!player || !map) return;

	// Recalcular el camino si el jugador cambia de tile
	updatePathToPlayer();

	// Seguir el camino actual
	followPath(deltaTime);
}

void Enemy::updatePathToPlayer()
{
	if (!player || !map || !pathfinder) return;

	int tileSize = map->getTileSize();
	glm::ivec2 enemyTile = glm::ivec2(posEnemy.x / tileSize, posEnemy.y / tileSize);
	glm::ivec2 playerTile = glm::ivec2(player->getPosition().x / tileSize, player->getPosition().y / tileSize);

	// Evita recalcular si el jugador sigue en el mismo tile
	if (playerTile == lastPlayerTile && !path.empty()) return;
	lastPlayerTile = playerTile;

	std::vector<Node> newPath = pathfinder->findPath(enemyTile, playerTile);

	path.clear();
	for (auto& n : newPath)
		path.push_back(n);
}


void Enemy::followPath(int deltaTime)
{
	if (path.empty() || !map) return;

	Node next = path.front();
	glm::vec2 targetPos = glm::vec2(next.pos.x * map->getTileSize(), next.pos.y * map->getTileSize());
	glm::vec2 diff = targetPos - posEnemy;

	// Avanza al siguiente nodo si está cerca
	if (glm::length(diff) < 1.0f) {
		posEnemy = glm::vec2(next.pos.x * map->getTileSize(),
			next.pos.y * map->getTileSize());
		path.pop_front();
		if (path.empty()) {
			stopMovingAnim();
			return;
		}
		next = path.front();
		targetPos = glm::vec2(next.pos.x * map->getTileSize(), next.pos.y * map->getTileSize());
		diff = targetPos - posEnemy;
	}

	glm::vec2 dir = glm::normalize(diff);
	float speed = 0.08f; 
	posEnemy += dir * speed * float(deltaTime);

	changeDirAnim(dir);

	//sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x,tileMapDispl.y + posEnemy.y));
}

void Enemy::stopMovingAnim() {
	switch (facing) {
	case FACE_LEFT:  sprite->changeAnimation(ENEMY_STAND_LEFT);  break;
	case FACE_RIGHT: sprite->changeAnimation(ENEMY_STAND_RIGHT); break;
	case FACE_UP:    sprite->changeAnimation(ENEMY_STAND_UP);    break;
	case FACE_DOWN:  sprite->changeAnimation(ENEMY_STAND_DOWN);  break;
	}
}


void Enemy::changeDirAnim(glm::vec2 dir) {
	EnemyAnim newAnim;
	if (fabs(dir.x) > fabs(dir.y)) {
		if (dir.x > 0) { newAnim = ENEMY_MOVE_RIGHT; facing = FACE_RIGHT; }
		else { newAnim = ENEMY_MOVE_LEFT; facing = FACE_LEFT; }
	}
	else {
		if (dir.y > 0) { newAnim = ENEMY_MOVE_DOWN; facing = FACE_DOWN; }
		else { newAnim = ENEMY_MOVE_UP; facing = FACE_UP; }
	}

	if (sprite->animation() != newAnim)
		sprite->changeAnimation(newAnim);
}


void Enemy::attack(int deltaTime)
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
