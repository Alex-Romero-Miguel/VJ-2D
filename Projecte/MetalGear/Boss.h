#pragma once
#include "Enemy.h"

// Machine gun kid. Mover en linea horizontal y disparar al jugador.

class Boss :
    public Enemy
{
public:
	enum BossAnim {
		BOSS_STAND, 
		BOSS_MOVE,
		BOSS_SHOOT
	};

	enum BossState {
		IDLE,
		CHOOSING_TARGET, 
		MOVING,
		ATTACKING,
		COOLDOWN, 
		DEAD
	};

	BossState state = IDLE;

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	//void resetState() override;

	//bool readyToShoot() const;

	bool attack(int deltaTime) override;

	bool isBoss() const override { return true; };


private: 

	//bool attack(int deltaTime) override;
	//void changeDirAnim(glm::vec2 dir) override;
	//void stopMovingAnim() override;

	//bool attack(int deltaTime) override;

	ShaderProgram* shaderProgram;

	vector<glm::vec2> shootingPoints;

	int burstShotsFired;

	int timeBetweenShots;

	glm::vec2 targetPoint;

	//Projectile shoot();


	glm::vec2 findClosestShootingPoint();


};

