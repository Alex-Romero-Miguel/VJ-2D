#include "Weapon.h"

Weapon::Weapon(ShaderProgram *shaderProgram)
{
    inWorldSpritesheet.loadFromFile("images/items_weapons_other_transparent.png", TEXTURE_PIXEL_FORMAT_RGBA);

    inWorldSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.125f), &inWorldSpritesheet, shaderProgram);
    inWorldSprite->setNumberAnimations(1);
    inWorldSprite->setAnimationSpeed(0, 1);
    inWorldSprite->addKeyframe(0, glm::vec2(0.0f, 0.375f));
    inWorldSprite->changeAnimation(0);

    inHudSpritesheet.loadFromFile("images/hud_elements.png", TEXTURE_PIXEL_FORMAT_RGBA);

    inHudSprite = Sprite::createSprite(glm::ivec2(72, 8), glm::vec2(0.05625f, 0.0203125f), &inHudSpritesheet, shaderProgram);
    inHudSprite->setNumberAnimations(1);
    inHudSprite->setAnimationSpeed(0, 1);
    inHudSprite->addKeyframe(0, glm::vec2(0.0f, 0.525f));
    inHudSprite->changeAnimation(0);
}

Weapon *Weapon::createWeapon(ShaderProgram *shaderProgram)
{
    Weapon *weapon = new Weapon(shaderProgram);

    return weapon;
}

Weapon::~Weapon()
{
    
}

void Weapon::effect()
{
    
}