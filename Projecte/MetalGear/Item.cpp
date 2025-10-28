#include "Item.h"

Item::Item()
{
    sprite = NULL;
}

Item::~Item()
{
    if(sprite != NULL)
        delete sprite;
}

void Item::init(const int ts, const glm::ivec2 &tileMapPos)
{
    tileSize = ts;
    tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::render()
{
    if (!picked)
        sprite->render();
}

void Item::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2 &pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

bool Item::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    return true;
}

bool Item::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    return true;
}

bool Item::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    return true;
}

bool Item::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    return true;
}

void Item::pickUp()
{
    picked = true;
}

void Item::use(Player *player) 
{
    effect(player);
}