//#include "Proyectile.h"
//#include <glm/gtc/matrix_transform.hpp>
//
//Projectile::Projectile(const glm::vec2& startPos, const glm::vec2& dir, float spd, int dmg, TileMap* tilemap)
//	: pos(startPos), direction(glm::normalize(dir)), speed(spd), damage(dmg), map(tilemap), alive(true) {}
//
//void Projectile::update(int deltaTime)
//{
//	if (!alive) return;
//
//	pos += direction * speed * float(deltaTime);
//
//	// Verificar colisión con el mapa
//	if (map && map->collisionMoveLeft(pos, glm::ivec2(4, 4))
//		|| map->collisionMoveRight(pos, glm::ivec2(4, 4))
//		|| map->collisionMoveDown(pos, glm::ivec2(4, 4), nullptr, nullptr))
//		//|| map->collisionMoveUp(pos, glm::ivec2(4, 4), nullptr, nullptr))
//	{
//		alive = false;
//	}
//}
//
//void Projectile::render()
//{
//	if (!alive) return;
//	glColor3f(1.0f, 0.2f, 0.2f);
//	glBegin(GL_QUADS);
//	glVertex2f(pos.x, pos.y);
//	glVertex2f(pos.x + 4, pos.y);
//	glVertex2f(pos.x + 4, pos.y + 4);
//	glVertex2f(pos.x, pos.y + 4);
//	glEnd();
//	glColor3f(1.f, 1.f, 1.f);
//}