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
#include <map>
#include <vector>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

struct Door {
	glm::ivec2 pos;				// esquina superior de la puerta, en tiles
	glm::ivec2 size;			// tamaño puerta en tiles
	std::string targetLevel;	// archivo de nivel de destino
	glm::ivec2 spawnPos;        // posición de aparición
	int levelID;
	//std::string instanceTag;    // identificador lógico
	//std::string returnTag;      // a qué punto del mapa exterior vuelve
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();

	void loadLevel(const string& levelFile, const glm::ivec2& playerSpawnPos);
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

private:
	void initShaders();

	string levelFile;
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

	glm::ivec2 playerStartPos;

	Rations *rations;

	std::vector<Door> doors;

	int doorCooldown;

	std::map<int, std::vector<Door>> allDoors;
	int currentLevel = 1;
};


#endif // _SCENE_INCLUDE

