#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <iostream>
#include "Sprite.h"
#include "TileMap.h"
#include "Item.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP, STAND_DOWN,
	MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
	PUNCH_LEFT, PUNCH_RIGHT, PUNCH_UP, PUNCH_DOWN
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

	glm::ivec2 getPosition() const { return posPlayer; }
	glm::ivec2 getSize() const { return glm::ivec2(16, 32); }

	void takeDamage(int amount);
	void heal(int amount);

	void pickUpItem(Item *item);
	void useItem();
	void changeItem();
	void consumeItem();

	bool isDead() const;
	int getHealth() const;
	float getHealthPercentage() const;
	
private:
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	vector<Item*> inventory;
	int current_item = 0;
	// Weapon *weapon;

	FacingDir facing;

	bool zWasDown = false;
	bool punching = false;
	int  punchElapsedMs = 0;
	int  punchDurationMs = 250;

	int health; // Vida actual del jugador
	static const int STARTING_HEALTH = 3;
};


#endif // _PLAYER_INCLUDE


