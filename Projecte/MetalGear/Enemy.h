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
	virtual ~Enemy() {}

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);

	glm::vec2 getPosition() const { return posEnemy; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 32); }

private:
	

protected:
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

	bool canSeePlayer();
	void patrol();
	void chase(int deltaTime);

	void updatePathToPlayer();
	void followPath(int deltaTime);
	virtual void attack(int deltaTime);


	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy;     // posición en píxeles (float para suavidad)
	glm::ivec2 patrolStart, patrolEnd, currentPatrolTarget;

	float attackRange = 100.f;

	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player* player;

	float visionRange = 300.f; // rango de visión
	float visionAngle = 45.f;  // en grados

	std::deque<Node> path;  // ruta actual

	std::unique_ptr<AStar> pathfinder;
	glm::ivec2 lastPlayerTile;

	int shootCooldown = 0;         // tiempo entre disparos (ms)
	int shootRate = 1200;          // cada cuánto puede disparar (1.2s)
	float bulletSpeed = 0.3f;      // velocidad del proyectil
	bool isShooting = false;


	void stopMovingAnim();
	virtual void changeDirAnim(glm::vec2 dir);

};

#endif // _ENEMY_INCLUDE




