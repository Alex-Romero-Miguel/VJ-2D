#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <iostream>
#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include "Item.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Item;
class Weapon;

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN,
	MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
	PUNCH_LEFT, PUNCH_RIGHT, PUNCH_UP, PUNCH_DOWN, DEAD
};

enum FacingDir { FACE_LEFT, FACE_RIGHT, FACE_UP, FACE_DOWN };


class Player
{

public:
	Player();
	~Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void resetHealth() { health = STARTING_HEALTH; dead = false; }

	void takeDamage(int amount);

	glm::ivec2 getPosition() const { return posPlayer; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 32); }
	glm::ivec4 getPunchHitbox() const;

	void heal(int amount);

	void pickUpItem(Item *item);
	void pickUpWeapon(Weapon *weapon);
	void useItem();
	void changeItem();
	void consumeItem();

	bool isDead() const;
	int getHealth() const;
	float getHealthPercentage() const;
	Item* getCurrentItem() const;
	Weapon* getWeapon() const;
	
private:
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	ShaderProgram* shaderProgram;

	vector<Item*> inventory;
	int current_item = 0;

	Weapon *weapon;

	FacingDir facing;

	bool zWasDown = false;
	bool punching = false;
	int  punchElapsedMs = 0;
	int  punchDurationMs = 250;

	bool isHurt = false;
	int hurtTimer = 0;
	float hurtBlinkTime = 0.f;

	bool dead = false;

	static const int STARTING_HEALTH = 3;
	int health; // Vida actual del jugador
};


#endif // _PLAYER_INCLUDE


