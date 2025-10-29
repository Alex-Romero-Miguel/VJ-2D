#ifndef _WEAPON_INCLUDE
#define _WEAPON_INCLUDE

#include "Item.h"
#include "Player.h"

class Weapon : public Item 
{
private:
    Weapon(ShaderProgram *shaderProgram);

public:
    static Weapon *createWeapon(ShaderProgram *shaderProgram);
    ~Weapon();

    void effect() override;

private:
    int ammo = 1000;
};

#endif // _WEAPON_INCLUDE