#include "HUD.h"
#include "Game.h"

#define HEALTH_BAR_X_TILES 5
#define HEALTH_BAR_Y_TILES 0


HUD::HUD()
{
    background = NULL;
    playerReference = NULL;
}

HUD::~HUD()
{
    if(healthbar != NULL) 
        delete healthbar;
    if(background != NULL) 
        delete background;
}


void HUD::init(int ts, const glm::ivec2 &tileMapPos, const glm::vec2 &pos, Player *player, ShaderProgram &shaderProgram)
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

    healthbar = Sprite::createSprite(glm::ivec2(56, 8), glm::vec2(0.04375f, 0.021875f), &HBspritesheet, &shaderProgram);
    healthbar->setPosition(glm::vec2(origin.x + HEALTH_BAR_X_TILES * tileSize, origin.y + HEALTH_BAR_Y_TILES * tileSize));
    healthbar->setScale(glm::vec2(1.0f, 1.0f));
    healthbar->setNumberAnimations(1);
    healthbar->setAnimationSpeed(0, 1);
    healthbar->addKeyframe(0, glm::vec2(0.03125f, 0.628125f));
    healthbar->changeAnimation(0);

    playerReference = player;
    prevHP = playerReference->getHealthPercentage();
}

void HUD::update(int deltaTime)
{
    healthbar->update(deltaTime);
    // ammo->update(deltaTime);

    float HP = playerReference->getHealthPercentage();

    if (HP != prevHP) {
        healthbar->setScale(glm::vec2(float(HP), 1.0f));
    }
    prevHP = HP;
    
}

void HUD::render()
{
    background->render();
    healthbar->render();
    // ammo->render();
}

int HUD::getHeight() const
{
    return height;
}