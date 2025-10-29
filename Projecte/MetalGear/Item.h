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

    void init(const glm::ivec2 &tileMapPos, Player *playerReference);
    void update(int deltaTime);
    void renderInWorld();
    void renderInHud();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPosition() const { return posItem; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 16); }
    
	bool checkCollision(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    
    void pickUp();
    void use();
    virtual void effect() = 0;

protected:
    bool picked = false;
    glm::ivec2 tileMapDispl, posItem;
	Texture inWorldSpritesheet, inHudSpritesheet;
    Sprite  *inWorldSprite,     *inHudSprite;
	TileMap *map;
    Player *player;

};

#endif // _ITEM_INCLUDE