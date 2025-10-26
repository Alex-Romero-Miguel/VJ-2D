#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "AStar.h"
#include <deque>
#include <memory>
#include "Projectile.h"

class Enemy
{
public:
	virtual ~Enemy() {}

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime);
	virtual void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);

	glm::vec2 getPosition() const { return posEnemy; }
	virtual glm::ivec2 getSize() { return glm::ivec2(16, 32); };
	

protected:
	enum Facing { FACE_LEFT, FACE_RIGHT, FACE_UP, FACE_DOWN };

	// Posición y entorno
	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy;
	TileMap* map;
	Player* player;
	glm::ivec2 patrolStart, patrolEnd, currentPatrolTarget;

	// Sprites y animación
	Texture spritesheet;
	Sprite* sprite;
	Facing facing;
	

	// PathFinding
	std::deque<Node> path;
	std::unique_ptr<AStar> pathfinder;
	glm::ivec2 lastPlayerTile;


	// Estado y atributos 
	int health = 3;
	float visionRange = 300.f; // rango de visión
	float visionAngle = 45.f;  // en grados
	float attackRange = 100.f;


	// funciones
	bool canSeePlayer();
	void patrol();
	void chase(int deltaTime);
	void updatePathToPlayer();
	void followPath(int deltaTime);
	virtual void attack(int deltaTime);

	// Animaciones
	virtual void stopMovingAnim();
	virtual void changeDirAnim(glm::vec2 dir);

	
};

#endif // _ENEMY_INCLUDE




