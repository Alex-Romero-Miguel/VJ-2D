#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
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

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime = 0;
	glm::mat4 projection;
	std::vector<Enemy*> enemies;

	int deathTimer = 1000; 
	bool godMode = false;



};


#endif // _SCENE_INCLUDE

