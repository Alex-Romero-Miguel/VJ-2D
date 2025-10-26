#pragma once
#include "Enemy.h"
class Dog : public Enemy
{
public: 
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

	


private:
	enum DogAnim {
		DOG_IDLE,
		DOG_MOVE_UP,
		DOG_MOVE_DOWN,
		DOG_MOVE_LEFT,
		DOG_MOVE_RIGHT
	};

	Facing facing;


	void attack(int deltaTime) override;

	void changeDirAnim(glm::vec2 dir) override;
};

