#include "Item.h"

Item::Item()
{
    inWorldSprite = NULL;
    inHudSprite = NULL;
    map = NULL;
    player = NULL;
}

Item::~Item()
{
    if(inWorldSprite != NULL)
        delete inWorldSprite;
    if(inHudSprite != NULL)
        delete inHudSprite;
}

void Item::init(const glm::ivec2& tileMapPos, Player* playerReference)
{
    tileMapDispl = tileMapPos;
    player = playerReference;
}

void Item::update(int deltaTime)
{
    if (picked)
        inHudSprite->update(deltaTime);
    else
        inWorldSprite->update(deltaTime);
}

void Item::renderInWorld()
{
    if(!picked)
        inWorldSprite->render();
}

void Item::renderInHud()
{
    if(picked)
        inHudSprite->render();
}

void Item::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2 &pos)
{
    if(picked)
        inHudSprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
    else 
    {
        posItem = pos;
        inWorldSprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
    }
}

bool Item::checkCollision(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    bool overlap_x = pos.x < posItem.x + 16 && pos.x + size.x > posItem.x;
    bool overlap_y = pos.y < posItem.y + 16 && pos.y + size.y > posItem.y;
    return !picked && overlap_x && overlap_y;
}

void Item::pickUp()
{
    picked = true;
}

void Item::use() 
{
    effect();
}