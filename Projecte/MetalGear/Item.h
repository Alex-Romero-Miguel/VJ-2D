#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Player;

class Item 
{
public:
    Item();
    ~Item();

    void init(const int ts, const glm::ivec2 &tileMapPos);
    void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPosition() const { return posItem; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 16); }
    
	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
    
    void pickUp();
    void use(Player *player);
    virtual void effect(Player *player) = 0;

protected:
    bool picked;
    glm::ivec2 tileMapDispl, posItem;
	Texture spritesheet;
    Sprite *sprite;
	TileMap *map;
    int tileSize;

};

#endif // _ITEM_INCLUDE