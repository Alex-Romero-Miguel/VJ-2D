#pragma once
#include "Enemy.h"
class Dog : public Enemy
{
public: 
	enum DogAnim {
		DOG_IDLE,
		DOG_MOVE_UP,
		DOG_MOVE_DOWN,
		DOG_MOVE_LEFT,
		DOG_MOVE_RIGHT
	};

	enum DogState {
		PATROLLING,
		CHASING,
		ATTACKING,
		RETURNING,
		DEAD
	};

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	glm::ivec2 getSize() { return glm::ivec2(32, 32); };

	bool attack(int deltaTime) override;
	
private:
	DogState state = PATROLLING;

	void changeDirAnim(glm::vec2 dir) override;

	void stopMovingAnim() override;

	float biteRange = 32.f;   // rango de mordida
	int damage = 1;

	float deadTimer;

};

