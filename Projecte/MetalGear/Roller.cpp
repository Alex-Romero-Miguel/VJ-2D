//#include "Roller.h"
//
//void Roller::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
//{
//	spritesheet.loadFromFile("images/roller.png", TEXTURE_PIXEL_FORMAT_RGBA);
//	sprite = Sprite::createSprite(glm::ivec2(16, 128), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
//	sprite->setNumberAnimations(1);
//	sprite->setAnimationSpeed(0, 8);
//	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
//	sprite->changeAnimation(0);
//
//	tileMapDispl = tileMapPos;
//	posEnemy = glm::vec2(0, 0);
//	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));
//
//	enemyState = PATROLLING;
//	direction = glm::vec2(0.f, 1.f); // empieza bajando
//	speed = 0.12f;
//}
//
//void Roller::update(int deltaTime)
//{
//	move(deltaTime);
//	attack(deltaTime);
//	sprite->update(deltaTime);
//	sprite->setPosition(glm::vec2(tileMapDispl.x + posEnemy.x, tileMapDispl.y + posEnemy.y));
//}
//
//void Roller::move(int deltaTime)
//{
//	glm::vec2 newPos = posEnemy + direction * speed * float(deltaTime);
//
//	if (direction.y > 0 && map->collisionMoveDown(newPos, getSize(), nullptr, nullptr))
//		direction.y = -1.f;
//	else if (direction.y < 0 && map->collisionMoveUp(newPos, getSize(), nullptr, nullptr))
//		direction.y = 1.f;
//
//	posEnemy += direction * speed * float(deltaTime);
//}
//
//void Roller::attack(int deltaTime)
//{
//	if (!player) return;
//
//	if (shootCooldown > 0) shootCooldown -= deltaTime;
//	if (shootCooldown > 0) return;
//
//	glm::vec2 rollerCenter = glm::vec2(posEnemy.x + getSize().x / 2, posEnemy.y + getSize().y / 2);
//	glm::vec2 playerCenter = glm::vec2(player->getPosition().x + player->getSize().x / 2,
//		player->getPosition().y + player->getSize().y / 2);
//
//	float distance = glm::length(playerCenter - rollerCenter);
//	if (distance <= 20.0f)
//	{
//		player->takeDamage(2);
//		shootCooldown = 1000;
//	}
//}