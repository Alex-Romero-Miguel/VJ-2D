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
		IDLE,
		ALERTED,
		CHASING, 
		RETURNING,
		ATTACKING,
		DEAD
	};

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

	void resetState() override;

	bool isGuard() const { return true; };

	void setToAlarted();

	
private:
	GuardState state = IDLE;

	bool attack(int deltaTime) override;
	void stopMovingAnim() override;
	void changeDirAnim(glm::vec2 dir) override;

	glm::vec2 targetPos;

	int attackCooldown = 0;
};

