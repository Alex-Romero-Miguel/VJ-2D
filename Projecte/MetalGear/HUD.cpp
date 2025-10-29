#include "HUD.h"
#include "Game.h"

#define HEALTH_BAR_X_TILES 5
#define HEALTH_BAR_Y_TILES 0

#define WEAPON_X_TILES 18
#define WEAPON_Y_TILES 0

#define ITEM_X_TILES 18
#define ITEM_Y_TILES 2


HUD::HUD()
{
    background = NULL;
    player = NULL;
    item = NULL;
    weapon = NULL;
}

HUD::~HUD()
{
    if(healthbar != NULL) 
        delete healthbar;
    if(background != NULL) 
        delete background;
}


void HUD::init(int ts, const glm::ivec2 &tileMapPos, const glm::vec2 &pos, Player *playerReference, ShaderProgram &shaderProgram)
{
    height = 5;
    tileSize = ts;
    tileMapDispl = tileMapPos;
    posHUD = pos;

    glm::vec2 origin = glm::vec2(tileMapDispl.x + posHUD.x, tileMapDispl.y + posHUD.y);
    
    BGspritesheet.loadFromFile("images/hud_background.png", TEXTURE_PIXEL_FORMAT_RGBA);

    background = Sprite::createSprite(glm::ivec2(256, 40), glm::vec2(1.0f, 1.0f), &BGspritesheet, &shaderProgram);
    background->setPosition(origin);
    background->setScale(glm::vec2(1.0f, 1.0f));

    HBspritesheet.loadFromFile("images/hud_elements.png", TEXTURE_PIXEL_FORMAT_RGBA);

    healthbar = Sprite::createSprite(glm::ivec2(56, 8), glm::vec2(0.04375f, 0.025f), &HBspritesheet, &shaderProgram);
    healthbar->setPosition(glm::vec2(origin.x + HEALTH_BAR_X_TILES * tileSize, origin.y + HEALTH_BAR_Y_TILES * tileSize));
    healthbar->setScale(glm::vec2(1.0f, 1.0f));
    healthbar->setNumberAnimations(1);
    healthbar->setAnimationSpeed(0, 1);
    healthbar->addKeyframe(0, glm::vec2(0.03125f, 0.625f));
    healthbar->changeAnimation(0);

    player = playerReference;
    prevHP = player->getHealthPercentage();
}

void HUD::update(int deltaTime)
{
    healthbar->update(deltaTime);
    if(item != NULL) item->update(deltaTime);
    if(weapon != NULL) weapon->update(deltaTime);

    float HP = player->getHealthPercentage();

    if (HP != prevHP) {
        healthbar->setScale(glm::vec2(float(HP), 1.0f));
    }
    prevHP = HP;
    
}

void HUD::render()
{
    background->render();
    healthbar->render();
    if(item != NULL) item->renderInHud();
    if(weapon != NULL) weapon->renderInHud();
}

int HUD::getHeight() const
{
    return height;
}

void HUD::setItem(Item *i)
{
    item = i;
    if(item != NULL) item->setPosition(glm::vec2(tileMapDispl.x + posHUD.x + ITEM_X_TILES * tileSize, tileMapDispl.y + posHUD.y + ITEM_Y_TILES * tileSize));
}

void HUD::setWeapon(Weapon *w)
{
    weapon = w;
    if(weapon != NULL) weapon->setPosition(glm::vec2(tileMapDispl.x + posHUD.x + WEAPON_X_TILES * tileSize, tileMapDispl.y + posHUD.y + WEAPON_Y_TILES * tileSize));
}