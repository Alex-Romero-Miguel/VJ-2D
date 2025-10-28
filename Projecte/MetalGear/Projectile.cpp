#include "Projectile.h"


void Projectile::init(const glm::vec2& pos, const glm::vec2& dir, ShaderProgram& shaderProgram)
{
	posProjectile = pos;
	direction = dir;

	spritesheet.loadFromFile("images/projectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);

	sprite->setPosition(posProjectile);

}

void Projectile::update(int deltaTime, TileMap* map)
{
	lifetime -= deltaTime;
	if (lifetime <= 0) {
		toRemove = true;
		return;
	}
	posProjectile += direction * speed * float(deltaTime);

	// Comprueba si choca con una pared
	if (map->collisionMoveRight(posProjectile, getSize()) || map->collisionMoveLeft(posProjectile, getSize()) ||
		map->collisionMoveUp(posProjectile, getSize()) || map->collisionMoveDown(posProjectile, getSize()))
	{
		toRemove = true;
	}

	sprite->setPosition(posProjectile);
}

void Projectile::render()
{
	sprite->render();
}