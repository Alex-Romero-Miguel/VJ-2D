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

    void init(int ts, const glm::ivec2 &tileMapPos, const glm::vec2 &pos, Player *player, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileSize(int ts);
    int getHeight() const;

private:
    Texture BGspritesheet,  HBspritesheet;
    Sprite  *background,    *healthbar;

    // Sprite *ammo;
    
	glm::ivec2 tileMapDispl, posHUD;
    int height, tileSize;
    
    Player *playerReference;
    float prevHP;
};

#endif // _HUD_INCLUDE