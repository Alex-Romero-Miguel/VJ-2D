#ifndef _RATIONS_INCLUDE
#define _RATIONS_INCLUDE

#include "Item.h"
#include "Player.h"

class Rations : public Item 
{
private:
    Rations(ShaderProgram *shaderProgram);

public:
    static Rations *createRations(ShaderProgram *shaderProgram);
    ~Rations();

    void effect() override;

private:
    int healingAmount = 1000;
};

#endif // _RATIONS_INCLUDE