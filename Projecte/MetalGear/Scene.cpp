#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Guard.h"
#include "Dog.h"
#include "Camera.h"
#include "Boss.h"
#include "Guard.h"

#include <fstream>
#include <sstream>

#define SCREEN_X 0
#define SCREEN_Y 0

#define MAP_DISPLAY_X 256
#define MAP_DISPLAY_Y 192

#define INIT_PLAYER_X_TILES 16
#define INIT_PLAYER_Y_TILES 16

#define HUD_X_TILES 0
#define HUD_Y_TILES 0



Scene::Scene()
{
	map = NULL;
	player = NULL;
	hud = NULL;
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

	for (Projectile* p : projectiles) delete p;
	projectiles.clear();
	if(hud != NULL)
		delete hud; 
	for (Item* item : items)
	{
		delete item;
	}
	items.clear();
	
}


void Scene::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/interior.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	glm::ivec2 map_size_tiles = map->getMapSize();
	int tile_size = map->getTileSize();
	glm::ivec2 map_size = glm::ivec2(MAP_DISPLAY_X, MAP_DISPLAY_Y);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * tile_size, INIT_PLAYER_Y_TILES * tile_size));
	player->setTileMap(map);
	playerStartPos = glm::ivec2(15, 15);

	hud = new HUD();
	hud->init(tile_size, glm::ivec2(SCREEN_X, SCREEN_Y), glm::ivec2(HUD_X_TILES * tile_size, map_size.y + HUD_Y_TILES * tile_size),player, texProgram);
	
	Rations *rations = Rations::createRations(&texProgram);
	rations->init(glm::ivec2(SCREEN_X, SCREEN_Y), player);
	rations->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 4) * tile_size, INIT_PLAYER_Y_TILES * tile_size));
	rations->setTileMap(map);
	items.push_back(rations);

	Weapon *weapon = Weapon::createWeapon(&texProgram);
	weapon->init(glm::ivec2(SCREEN_X, SCREEN_Y), player);
	weapon->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 6) * tile_size, (INIT_PLAYER_Y_TILES + 1) * tile_size));
	weapon->setTileMap(map);
	items.push_back(weapon);
	
	/*
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
	}*/

	/*Camera* camera = new Camera();
	camera->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	camera->setPosition(glm::vec2(5 * map->getTileSize(),
		10 * map->getTileSize()));
	camera->setPatrolRoute(glm::ivec2(5 * map->getTileSize(), 10 * map->getTileSize()),
		glm::ivec2(10 * map->getTileSize(), 10 * map->getTileSize()));
	camera->setTileMap(map);
	camera->setPlayer(player);
	enemies.push_back(camera);*/

	/*
	Dog* dog = new Dog();
	dog->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	dog->setPosition(glm::vec2(10 * map->getTileSize(),
		10 * map->getTileSize()));
	dog->setTileMap(map);
	dog->setPlayer(player);
	enemies.push_back(dog);*/
	

	currentLevel = 1;
	// currentTime = 0.0f;
	cameraPos = glm::vec2(0,0);
	loadLevel("levels/exterior.txt", glm::vec2(15,15));
	playerStartPos = glm::ivec2(15, 15);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH+ SCREEN_X), float(SCREEN_HEIGHT+ SCREEN_Y + +(hud->getHeight() * tile_size)), 0.f);
	deathTimer = 1000;

	allDoors.clear();
	// Cargá las puertas del Nivel 1 (exterior)
	allDoors[1] = {
		{glm::ivec2(54, 12), glm::ivec2(2, 3), "levels/truck.txt", glm::ivec2(20, 11), 4},  // El '4' es el ID del nivel de destino
		{glm::ivec2(84, 51), glm::ivec2(4, 4), "levels/interior.txt", glm::ivec2(47, 67), 2}
	};

	// Cargá las puertas del Nivel 2 (interior)
	allDoors[2] = {
		{glm::ivec2(56, 8), glm::ivec2(1, 3), "levels/room.txt", glm::ivec2(13, 10), 4},
		{glm::ivec2(9, 19), glm::ivec2(2, 3), "levels/boss.txt", glm::ivec2(15, 15), 3}
	};

	// Cargá las puertas del Nivel 4 (camión)
	allDoors[4] = {
		{glm::ivec2(24, 10), glm::ivec2(4, 8), "levels/exterior.txt", glm::ivec2(56, 14), 1}
	};
	// Cargá las puertas del Nivel 5 (room)
	allDoors[5] = {
		{glm::ivec2(4, 8), glm::ivec2(2, 2), "levels/interior.txt", glm::ivec2(50, 8), 2}
	};


}

void Scene::loadLevel(const string& levelFile, const glm::ivec2& playerSpawnPos)
{
	// Limpiar recursos antiguos
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
	if (map != NULL) delete map;

	map = TileMap::createTileMap(levelFile, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setTileMap(map);

	player->setPosition(glm::vec2(
		playerSpawnPos.x * map->getTileSize(),
		playerSpawnPos.y * map->getTileSize()
	));

	if (levelFile == "levels/exterior.txt") {

		currentLevel = 1;
	}
	else if (levelFile == "levels/interior.txt") {
		currentLevel = 2;
		
	}
	else if (levelFile == "levels/boss.txt") {
		

		currentLevel = 3;
	}
	else if (levelFile == "levels/truck.txt") {

		currentLevel = 4;

	}
	else if (levelFile == "levels/room.txt") {
		currentLevel = 5;
	}	

	loadEnemiesFromFile(levelFile);
}

void Scene::loadEnemiesFromFile(const string& levelFile)
{
	ifstream fin(levelFile);
	string line;

	// Busca la secci�n [ENEMIES] en el archivo
	while (getline(fin, line) && line.find("ENEMIES") == string::npos) {
		// Sigue leyendo hasta encontrar la secci�n o el final del archivo
	}

	// Si se encontr�, empieza a leer los datos de los enemigos
	string enemyType;
	int startX, startY;
	while (fin >> enemyType) {
		fin >> startX >> startY;

		Enemy* newEnemy = nullptr;

		if (enemyType == "guard") {
			newEnemy = new Guard();
			/*int endX, endY;
			fin >> endX >> endY;*/
			newEnemy->setPatrolRoute(glm::ivec2(startX * map->getTileSize(), startY * map->getTileSize()),
				glm::ivec2(startX * map->getTileSize(), startY * map->getTileSize()));
		}
		else if (enemyType == "dog") {
			newEnemy = new Dog();
		}
		else if (enemyType == "camera") {
			Camera* camera = new Camera();
			int endX, endY;
			fin >> endX >> endY;

			camera->setPatrolRoute(
				glm::ivec2(startX * map->getTileSize(), startY * map->getTileSize()),
				glm::ivec2(endX * map->getTileSize(), endY * map->getTileSize())
			);
			camera->setEnemies(&enemies);
			newEnemy = camera;
		}
		else if (enemyType == "boss") {
			Boss* boss = new Boss();
			newEnemy = boss;
		}

		// Si se cre� un enemigo v�lido, inicial�zalo y a��delo a la lista
		if (newEnemy != nullptr) {
			newEnemy->init(glm::ivec2(0, 0), texProgram);
			newEnemy->setPosition(glm::vec2(startX * map->getTileSize(), startY * map->getTileSize()));
			newEnemy->setTileMap(map);
			newEnemy->setPlayer(player);
			enemies.push_back(newEnemy);
		}
	}
}

void Scene::restartGame()
{
	// Borrar enemigos
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();

	delete player;
	delete map;

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
	// Si el jugador esta muerto, reiniciar el juego
	if (player->isDead()) {
		deathTimer -= deltaTime;
		if (deathTimer <= 0)
			restartGame();
		return;
	}


	const int ts = map->getTileSize();
	glm::ivec4 playerBox(player->getPosition(), player->getSize());
	glm::vec2 playerPos = player->getPosition();

	bool changeNivel = false;

	const std::vector<Door>& currentDoors = allDoors[currentLevel];
	for (const Door& d : currentDoors)
	{
		if (checkCollision(
			glm::ivec4(player->getPosition(), player->getSize()),
			glm::ivec4(d.pos.x * map->getTileSize(), d.pos.y * map->getTileSize(), d.size.x * map->getTileSize(), d.size.y * map->getTileSize())
		))
		{
			loadLevel(d.targetLevel, d.spawnPos);
			return;
		}
	}

	/*if (!changeNivel) {*/
		// Si el jugador se sale por la derecha
		if (playerPos.x + player->getSize().x > cameraPos.x + SCREEN_WIDTH) {
			cameraPos.x += SCREEN_WIDTH; // Mueve la c�mara una pantalla a la derecha
			player->setPosition(glm::vec2(playerPos.x + player->getSize().x + 1, playerPos.y)); // Coloca al jugador al inicio de la nueva pantalla
		}
		// Si el jugador se sale por la izquierda
		else if (playerPos.x < cameraPos.x) {
			cameraPos.x -= SCREEN_WIDTH; // Mueve la c�mara a la izquierda
			player->setPosition(glm::vec2(playerPos.x - player->getSize().x - 1, playerPos.y)); // Coloca al jugador al final de la nueva pantalla
		}
		// Si el jugador se sale por abajo
		else if (playerPos.y + player->getSize().y > cameraPos.y + SCREEN_HEIGHT) {
			cameraPos.y += SCREEN_HEIGHT; // Mueve la c�mara una pantalla hacia abajo
			player->setPosition(glm::vec2(playerPos.x, playerPos.y + player->getSize().y + 1)); // Coloca al jugador al inicio de la nueva pantalla
		}
		// Si el jugador se sale por arriba
		else if (playerPos.y < cameraPos.y) {
			cameraPos.y -= SCREEN_HEIGHT; // Mueve la c�mara una pantalla hacia arriba
			player->setPosition(glm::vec2(playerPos.x, playerPos.y - player->getSize().y - 1));
		}
	//}

	

	// Actualizamos al jugador
	player->update(deltaTime);

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* enemy = *it;

		// Comprueba si el enemigo está en la pantalla actual
		bool isOnScreen = (enemy->getPosition().x + enemy->getSize().x > cameraPos.x &&
			enemy->getPosition().x < cameraPos.x + 256.f &&
			enemy->getPosition().y + enemy->getSize().y > cameraPos.y &&
			enemy->getPosition().y < cameraPos.y + 192.f);

		// Actualiza la IA solo si está activo
		if (isOnScreen) {
			enemy->update(deltaTime);

			Guard* guard = dynamic_cast<Guard*>(enemy);
			Boss* boss = dynamic_cast<Boss*>(enemy);
			if ((guard != nullptr || boss != nullptr) && enemy->attack(deltaTime)) {
				Projectile* p = new Projectile();
				glm::vec2 dir = glm::normalize(glm::vec2(player->getPosition()) - enemy->getPosition());
				p->init(enemy->getPosition() + glm::vec2(8, 16), dir, texProgram);
				projectiles.push_back(p);
			}

			if (!enemy->isDead() && !player->isDead() && checkCollision(
				glm::ivec4(player->getPosition(), player->getSize()),
				glm::ivec4(enemy->getPosition(), enemy->getSize())))
			{
				// El perro muerde, el guardia hace daño por toque, etc.
				if (enemy->attack(deltaTime) && !godMode) {
					player->takeDamage(enemy->getDamage());
				}
			}
		}
		else {
			enemy->resetState();
		}

		glm::ivec4 hitbox = player->getPunchHitbox();
		if (hitbox.z > 0 && !enemy->isDead()) {
			if (checkCollision(hitbox, glm::ivec4(enemy->getPosition(), enemy->getSize()))) {
				enemy->takeDamage(1);
			}
		}

		// 4. Comprueba el ataque de contacto del enemigo (si está en pantalla)
		if (isOnScreen && !enemy->isDead() && !player->isDead()) {
			if (checkCollision(glm::ivec4(player->getPosition(), player->getSize()), glm::ivec4(enemy->getPosition(), enemy->getSize()))) {
				if (enemy->attack(deltaTime) && !godMode) {
					player->takeDamage(enemy->getDamage());
				}
			}
		}

		// 5. Elimina al enemigo si está muerto y marcado para ser borrado
		if (enemy->toRemove) {
			delete enemy;
			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = projectiles.begin(); it != projectiles.end();) {
		Projectile* p = *it;
		p->update(deltaTime, map);

		// Comprueba colisión con el jugador
		if (checkCollision(glm::ivec4(player->getPosition(), player->getSize()), glm::ivec4(p->getPosition(), p->getSize()))) {
			if (!godMode) player->takeDamage(1);
			p->toRemove = true; 
		}

		if (p->toRemove) {
			delete p;
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}

	for (Item* i : items) {
		if(i->checkCollision(player->getPosition() + glm::ivec2(0, 16), glm::ivec2(16, 16) )) {
			i->pickUp();
			if(typeid(i) == typeid(Weapon))
				player->pickUpWeapon((Weapon*)i);
			else
				player->pickUpItem(i);
		};
	}

	Item *item = player->getCurrentItem();
	if(item) hud->setItem(item);

	Weapon *weapon = player->getWeapon();
	// if(weapon) hud->setWeapon(weapon);

	hud->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 view, model;

	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	
	
	view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.f));
	model = glm::mat4(1.0f);
	
	texProgram.setUniformMatrix4f("view", view);
	texProgram.setUniformMatrix4f("model", model);


	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	for (Item* i : items) i->renderInWorld();
	
	for (Enemy* enemy : enemies)
	{
		enemy->render();
	}

	
	for (Enemy* enemy : enemies)
	{
		enemy->render();
	}

	player->render();

	for (Projectile* p : projectiles) {
		p->render();
	}

	view = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("view", view);
	model = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("model", model);

	hud->render();
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
	loadLevel("levels/interior.txt", glm::vec2(47, 58));

	//player->setPosition(glm::vec2(5 * map->getTileSize(), 5 * map->getTileSize()));
}

void Scene::teleportToBoss() {
	if (!map || !player) return;
	
	loadLevel("levels/boss.txt", glm::vec2(15, 20));


	//player->setPosition(glm::vec2(40 * map->getTileSize(), 10 * map->getTileSize()));
}

