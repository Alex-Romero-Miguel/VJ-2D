#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Guard.h"
#include "Dog.h"
#include "Camera.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	const int NUM_ENEMIES = 3;

	glm::ivec2 enemyStartPositions[NUM_ENEMIES] = {
		glm::ivec2(10, 25),
		glm::ivec2(20, 20),
		glm::ivec2(15, 15)
	};
	
	for (int i = 0; i < NUM_ENEMIES; ++i)
	{
		Enemy* enemy = new Guard();

		enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		enemy->setPosition(glm::vec2(enemyStartPositions[i].x * map->getTileSize(),
			enemyStartPositions[i].y * map->getTileSize()));
		enemy->setTileMap(map);
		enemy->setPlayer(player);

		enemies.push_back(enemy); 
	}

	Camera* camera = new Camera();
	camera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	camera->setPosition(glm::vec2(5 * map->getTileSize(),
		10 * map->getTileSize()));
	camera->setPatrolRoute(glm::ivec2(5 * map->getTileSize(), 10 * map->getTileSize()),
		glm::ivec2(10 * map->getTileSize(), 10 * map->getTileSize()));
	camera->setTileMap(map);
	camera->setPlayer(player);
	enemies.push_back(camera);


	Dog* dog = new Dog();
	dog->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	dog->setPosition(glm::vec2(10 * map->getTileSize(),
		10 * map->getTileSize()));
	dog->setTileMap(map);
	dog->setPlayer(player);
	enemies.push_back(dog);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	deathTimer = 1000;
}


void Scene::restartGame()
{
	// Borrar enemigos
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();

	delete player;
	delete map;

	// Reiniciar todo igual que en init()
	init();
}

bool Scene::checkCollision(const glm::ivec4& a, const glm::ivec4& b)
{
	return (a.x < b.x + b.z &&
		a.x + a.z > b.x &&
		a.y < b.y + b.w &&
		a.y + a.w > b.y);
}



void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	// 1. Comprobamos si el jugador está muerto
	if (player->isDead()) {
		deathTimer -= deltaTime;
		if (deathTimer <= 0)
			restartGame();
		return;
	}

	// 2. Actualizamos al jugador
	player->update(deltaTime);

	// 3. Golpe del jugador (ataque cuerpo a cuerpo)
	glm::ivec4 hitbox = player->getPunchHitbox();
	if (hitbox.z > 0 && hitbox.w > 0) {
		for (Enemy* enemy : enemies) {
			if (enemy->isDead()) continue;

			glm::vec2 epos = enemy->getPosition();
			glm::ivec2 esize = enemy->getSize();
			if (checkCollision(hitbox, glm::ivec4(epos.x, epos.y, esize.x, esize.y))) {
				std::cout << "Golpeando enemigo en " << typeid(*enemy).name() << std::endl;
				enemy->takeDamage(1);
			}
		}
	}

	// 4. Actualizamos enemigos y comprobamos ataques
	for (auto it = enemies.begin(); it != enemies.end();) {
		Enemy* enemy = *it;
		enemy->update(deltaTime);

		// Comprobamos colisión de ataque o contacto
		if (!enemy->isDead() && checkCollision(
			glm::ivec4(player->getPosition().x, player->getPosition().y, player->getSize().x, player->getSize().y),
			glm::ivec4(enemy->getPosition().x, enemy->getPosition().y, enemy->getSize().x, enemy->getSize().y)))
		{
			// Si hay contacto y el enemigo ataca, y no hay godMode...
			if (enemy->attack(deltaTime) && !godMode)
				player->takeDamage(enemy->getDamage());
		}

		// Eliminamos enemigos muertos (tras su animación, si tienen)
		if (enemy->toRemove) {
			delete enemy;
			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	
	for (Enemy* enemy : enemies)
	{
		enemy->render();
	}

	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



void Scene::toggleGodMode() {
	godMode = !godMode;
	std::cout << (godMode ? "God mode ON" : "God mode OFF") << std::endl;
}

void Scene::fullHeal() {
	if (player) {
		player->resetHealth();
	}
}

void Scene::giveAllItems() {
	std::cout << "Todos los ítems añadidos (placeholder)" << std::endl;
	// Aquí puedes añadir objetos al inventario si ya tienes esa mecánica.
}

void Scene::teleportToInterior() {
	if (!map || !player) return;
	//player->setPosition(glm::vec2(5 * map->getTileSize(), 5 * map->getTileSize()));
}

void Scene::teleportToBoss() {
	if (!map || !player) return;
	//player->setPosition(glm::vec2(40 * map->getTileSize(), 10 * map->getTileSize()));
}


