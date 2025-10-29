#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

class HUD
{
public:
    HUD();
    ~HUD();

    void init(int tileSize, const glm::ivec2 &tileMapPos, const glm::vec2 &pos, Player *player, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileSize(int ts);
    int getHeight() const;
    void setItem(Item *item);
    void setWeapon(Weapon *weapon);

private:
    Texture BGspritesheet,  HBspritesheet;
    Sprite  *background,    *healthbar;
    Item *item;
    Weapon *weapon;

    // Sprite *ammo;
    
	glm::ivec2 tileMapDispl, posHUD;
    int height, tileSize;
    
    Player *player;
    float prevHP;
};

#endif // _HUD_INCLUDE