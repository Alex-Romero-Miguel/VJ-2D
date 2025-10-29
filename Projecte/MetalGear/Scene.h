#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <iostream>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "HUD.h"
#include "Rations.h"
#include "Weapon.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();

	void loadLevel(const string& levelFile);
	void loadEnemiesFromFile(const string& levelFile);

	void restartGame();

	void loadLevel(const string& levelFile);
	void loadEnemiesFromFile(const string& levelFile);

	void restartGame();
	void update(int deltaTime);
	void render();

	bool checkCollision(const glm::ivec4& a, const glm::ivec4& b);

	void toggleGodMode();
	void fullHeal();
	void giveAllItems();
	void teleportToInterior();
	void teleportToBoss();

	bool isGodMode() const { return godMode; }

	bool checkCollision(const glm::ivec4& a, const glm::ivec4& b);

	void toggleGodMode();
	void fullHeal();
	void giveAllItems();
	void teleportToInterior();
	void teleportToBoss();

	bool isGodMode() const { return godMode; }

private:
	void initShaders();

	TileMap *map;
	Player *player;
	HUD *hud;
	ShaderProgram texProgram;
	glm::mat4 projection;
	glm::vec2 cameraPos;
	std::vector<Enemy*> enemies;
	std::vector<Projectile*> projectiles;

	int deathTimer = 1000; 
	bool godMode = false;

	int currentLevel = 1;


	glm::ivec2 playerStartPos;

	vector<Item*> items;

};


#endif // _SCENE_INCLUDE

