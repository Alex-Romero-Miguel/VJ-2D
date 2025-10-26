#include "Enemy.h"
#include "Game.h"
#include <iostream>

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
}

void Enemy::update(int deltaTime)
{
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
	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));
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

	glm::vec2 current = enemyCenter;
	while (glm::distance(current, playerCenter) > step)
	{
		current += dir * step;

		// Convertimos la posición a coordenadas de tile
		int tileX = int(current.x) / map->getTileSize();
		int tileY = int(current.y) / map->getTileSize();

		// Si el tile no es walkable, significa que hay una pared
		if (!map->isWalkable(tileX, tileY))
			return false; // visión bloqueada
	}

	return true;
}

void Enemy::patrol()
{
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
}

void Enemy::stopMovingAnim() {
}


void Enemy::changeDirAnim(glm::vec2 dir) {
}


void Enemy::attack(int deltaTime)
{

}
