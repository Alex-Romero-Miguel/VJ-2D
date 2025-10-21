#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "AStar.h"
#include <deque>
#include <memory>

class Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);

	bool canSeePlayer();

	glm::vec2 getPosition() const { return posEnemy; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 32); }

	enum Facing { FACE_LEFT, FACE_RIGHT, FACE_UP, FACE_DOWN } facing;

	enum EnemyState {
		PATROLLING,
		ALERTED,
		CHASING,
		RETURNING,
		ATTACKING, 
		DEAD
	} enemyState;

	enum EnemyAnim {
		ENEMY_STAND_LEFT,
		ENEMY_STAND_RIGHT,
		ENEMY_STAND_UP,
		ENEMY_STAND_DOWN,
		ENEMY_MOVE_LEFT,
		ENEMY_MOVE_RIGHT,
		ENEMY_MOVE_UP,
		ENEMY_MOVE_DOWN,
		ENEMY_ATACCK,
		ENEMY_DIE
	};

	void updatePathToPlayer();
	void followPath(int deltaTime);


private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy;     // posición en píxeles (float para suavidad)
	glm::ivec2 patrolStart, patrolEnd, currentPatrolTarget;

	float attackRange = 100.f;

	void patrol();
	void chase(int deltaTime);

	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player* player;

	float visionRange = 300.f; // rango de visión
	float visionAngle = 45.f;  // en grados

	std::deque<Node> path;  // ruta actual
	int pathCooldown;       // tiempo entre recalculos de path

	glm::ivec2 lastKnownPlayerTile;
	float lostSightTime = 3.f;

	std::unique_ptr<AStar> pathfinder;
	glm::ivec2 lastPlayerTile;
};

#endif // _ENEMY_INCLUDE




