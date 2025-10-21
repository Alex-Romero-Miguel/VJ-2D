#pragma once
#include "Enemy.h"
class Dog :
    public Enemy
{
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

