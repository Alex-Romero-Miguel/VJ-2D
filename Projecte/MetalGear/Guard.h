#pragma once
#include "Enemy.h"
class Guard :
    public Enemy
{
public: 

	enum GuardAnim {
		GUARD_STAND_LEFT,
		GUARD_STAND_RIGHT,
		GUARD_STAND_UP,
		GUARD_STAND_DOWN,
		GUARD_MOVE_LEFT,
		GUARD_MOVE_RIGHT,
		GUARD_MOVE_UP,
		GUARD_MOVE_DOWN,
		GUARD_ATACCK,
		GUARD_DEAD
	};

	enum GuardState {
		PATROLLING,
		ALERTED,
		CHASING, 
		RETURNING,
		ATTACKING,
		DEAD
	};

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

	glm::ivec2 getSize() const { return glm::ivec2(16, 32); } 

	void resetState() override;

	
private:
	GuardState state = PATROLLING;

	bool attack(int deltaTime) override;
	void stopMovingAnim() override;
	void changeDirAnim(glm::vec2 dir) override;

	int attackCooldown = 0;
};

