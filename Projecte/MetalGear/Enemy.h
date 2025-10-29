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
	enum Facing { FACE_LEFT, FACE_RIGHT, FACE_UP, FACE_DOWN };

	virtual ~Enemy() {}

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime);
	virtual void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);
	void setFacing(Facing dir) { facing = dir; }

	virtual void resetState();

	glm::vec2 getPosition() const { return posEnemy; }
	virtual glm::ivec2 getSize() { return glm::ivec2(16, 32); };


	void takeDamage(int amount);
	bool isDead() const;

	bool toRemove = false;

	virtual bool attack(int deltaTime) { return false; };
	virtual int getDamage() const { return damage; };

	bool isMovable() const { return movable; }

	void setPatrolRoute(const glm::ivec2& start, const glm::ivec2& end);


	virtual bool isBoss() const { return false; };

	

protected:
	// Posición y entorno
	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy;
	TileMap* map;
	Player* player;
	glm::vec2 patrolStart, patrolEnd, currentPatrolTarget;

	glm::vec2 moveDir;

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
	bool dead = false; 
	bool isHurt = false;
	int hurtTimer = 0;

	bool knockUp = false;
	float verticalVel = 0.f;
	float verticalPos = 0.f;
	float moveSpeed = 0.1f;

	int damage = 1;

	
	int deadTimer = 0;

	float visionRange = 300.f; // rango de visión
	float visionAngle = 45.f;  // en grados
	float attackRange = 100.f;

	int attackCooldown = 0;


	bool movable = true;

	// funciones
	bool canSeePlayer();
	void patrol(int deltaTime);
	void chase(int deltaTime);
	void updatePathToPlayer();
	void followPath(int deltaTime);
	
	// Animaciones
	virtual void stopMovingAnim();
	virtual void changeDirAnim(glm::vec2 dir);

	
};

#endif // _ENEMY_INCLUDE




