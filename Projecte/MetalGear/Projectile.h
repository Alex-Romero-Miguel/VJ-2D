#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Projectile
{
public:
	void init(const glm::vec2& pos, const glm::vec2& dir, ShaderProgram& shaderProgram);
	void update(int deltaTime, TileMap* map);
	void render();

	glm::ivec2 getSize() const { return glm::ivec2(8, 8); }
	glm::vec2 getPosition() const { return posProjectile; }

	bool toRemove = false;

private:
	glm::vec2 posProjectile;
	glm::vec2 direction;
	float speed = 0.5f;
	Sprite* sprite;
	int lifetime = 2000; // La bala se destruye después de 2 segundos

	Texture spritesheet;
	
};

#endif // _PROJECTILE_INCLUDE