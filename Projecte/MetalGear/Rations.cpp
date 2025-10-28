#include "Rations.h"

Rations::Rations(ShaderProgram *shaderProgram)
{
    spritesheet.loadFromFile("images/items_weapons_other_transparent.png", TEXTURE_PIXEL_FORMAT_RGBA);

    sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.125f), &spritesheet, shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.0f, 0.125f));
    sprite->changeAnimation(0);

    picked = false;
}

Rations *Rations::createRations(ShaderProgram *shaderProgram)
{
    Rations *rations = new Rations(shaderProgram);

    return rations;
}

Rations::~Rations()
{
    
}

void Rations::effect(Player *player)
{
    player->heal(healingAmount);
    player->consumeItem();
}