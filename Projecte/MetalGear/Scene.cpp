#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Guard.h"
#include "Dog.h"
#include "Camera.h"
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
	currentLevel = 1;
	initShaders();

	map = TileMap::createTileMap("levels/interior.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	glm::ivec2 map_size_tiles = map->getMapSize();
	int tile_size = map->getTileSize();
	glm::ivec2 map_size = glm::ivec2(MAP_DISPLAY_X, MAP_DISPLAY_Y);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * tile_size, INIT_PLAYER_Y_TILES * tile_size));
	player->setTileMap(map);

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
	loadLevel("levels/exterior.txt");
	playerStartPos = glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH+ SCREEN_X), float(SCREEN_HEIGHT+ SCREEN_Y)+(hud->getHeight() * tile_size), 0.f);
	deathTimer = 1000;
}

void Scene::loadLevel(const string& levelFile)
{
	// 1. Limpiar recursos antiguos
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();

	if (levelFile == "levels/exterior.txt") {
		currentLevel = 1;
		playerStartPos = glm::ivec2(15, 15);
	}
	else if (levelFile == "levels/interior.txt") {
		currentLevel = 2;
		playerStartPos = glm::ivec2(50, 50);
	}
	else if (levelFile == "levels/boss.txt") {
		currentLevel = 3;
	}

	map = TileMap::createTileMap(levelFile, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setTileMap(map);
	player->setPosition(glm::vec2(playerStartPos.x * map->getTileSize(), playerStartPos.y * map->getTileSize()));


	////  Cargar los enemigos del nuevo nivel (�esto deber�a leerse del archivo del nivel!)
	//// Por ahora, lo ponemos como ejemplo:
	/*if (levelFile == "levels/exterior.txt") {
		Guard* guard = new Guard();
		guard->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		guard->setPosition(glm::vec2(10 * map->getTileSize(), 35 * map->getTileSize()));
		guard->setTileMap(map);
		guard->setPlayer(player);
		enemies.push_back(guard);
	}*/

	//loadPlayerStartPosition(levelFile);

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

		// Usa un "factory" para crear el tipo de enemigo correcto
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

			// Solo haz la configuraci�n espec�fica de la c�mara aqu�
			camera->setPatrolRoute(
				glm::ivec2(startX * map->getTileSize(), startY * map->getTileSize()),
				glm::ivec2(endX * map->getTileSize(), endY * map->getTileSize())
			);
			newEnemy = camera;
			/*camera->setPatrolRoute(glm::ivec2(5 * map->getTileSize(), 10 * map->getTileSize()),
				glm::ivec2(10 * map->getTileSize(), 10 * map->getTileSize()));*/

			/*newEnemy = camera;
			enemies.push_back(newEnemy);*/

			/*camera->setPosition(glm::vec2(x * map->getTileSize(),
				y * map->getTileSize()));

			camera->setTileMap(map);
			camera->setPlayer(player);
			enemies.push_back(camera);*/
		}

		// Si se cre� un enemigo v�lido, inicial�zalo y a��delo a la lista
		if (newEnemy != nullptr) {
			newEnemy->init(glm::ivec2(0, 0), texProgram);
			//if (!newEnemy->isMovable()) { 
			newEnemy->setPosition(glm::vec2(startX * map->getTileSize(), startY * map->getTileSize()));
			//}
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
	// Comprobamos si el jugador est� muerto
	if (player->isDead()) {
		deathTimer -= deltaTime;
		if (deathTimer <= 0)
			restartGame();
		return;
	}

	glm::vec2 playerPos = player->getPosition();

	// Si el jugador se sale por la derecha
	if (playerPos.x + player->getSize().x > cameraPos.x + SCREEN_WIDTH) {
		cameraPos.x +=SCREEN_WIDTH; // Mueve la c�mara una pantalla a la derecha
		player->setPosition(glm::vec2(playerPos.x + player->getSize().x + 1, playerPos.y)); // Coloca al jugador al inicio de la nueva pantalla
	}
	// Si el jugador se sale por la izquierda
	else if (playerPos.x < cameraPos.x ) {
		cameraPos.x -=SCREEN_WIDTH; // Mueve la c�mara a la izquierda
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

	// Actualizamos al jugador
	// Comprobamos si el jugador est� muerto
	if (player->isDead()) {
		deathTimer -= deltaTime;
		if (deathTimer <= 0)
			restartGame();
		return;
	}

	glm::vec2 playerPos = player->getPosition();

	// Si el jugador se sale por la derecha
	if (playerPos.x + player->getSize().x > cameraPos.x + SCREEN_WIDTH) {
		cameraPos.x +=SCREEN_WIDTH; // Mueve la c�mara una pantalla a la derecha
		player->setPosition(glm::vec2(playerPos.x + player->getSize().x + 1, playerPos.y)); // Coloca al jugador al inicio de la nueva pantalla
	}
	// Si el jugador se sale por la izquierda
	else if (playerPos.x < cameraPos.x ) {
		cameraPos.x -=SCREEN_WIDTH; // Mueve la c�mara a la izquierda
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

	// Actualizamos al jugador
	player->update(deltaTime);

	for (auto it = enemies.begin(); it != enemies.end(); /* El incremento se hace dentro */)
	{
		Enemy* enemy = *it;

		// 1. Comprueba si el enemigo est� en la pantalla actual
		bool isOnScreen = (enemy->getPosition().x + enemy->getSize().x > cameraPos.x &&
			enemy->getPosition().x < cameraPos.x + 256.f &&
			enemy->getPosition().y + enemy->getSize().y > cameraPos.y &&
			enemy->getPosition().y < cameraPos.y + 192.f);

		// 2. Actualiza la IA solo si est� activo
		if (isOnScreen) {
			enemy->update(deltaTime);
		}
		else {
			enemy->resetState();
		}

		// 3. Comprueba el ataque del jugador (pu�etazo)
		glm::ivec4 hitbox = player->getPunchHitbox();
		if (hitbox.z > 0 && !enemy->isDead()) {
			if (checkCollision(hitbox, glm::ivec4(enemy->getPosition(), enemy->getSize()))) {
				enemy->takeDamage(1);
			}
		}

		// 4. Comprueba el ataque de contacto del enemigo (si est� en pantalla)
		if (isOnScreen && !enemy->isDead() && !player->isDead()) {
			if (checkCollision(glm::ivec4(player->getPosition(), player->getSize()), glm::ivec4(enemy->getPosition(), enemy->getSize()))) {
				if (enemy->attack(deltaTime) && !godMode) {
					player->takeDamage(enemy->getDamage());
				}
			}
		}

		// 5. Elimina al enemigo si est� muerto y marcado para ser borrado
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

		// Comprueba colisi�n con el jugador
		if (checkCollision(glm::ivec4(player->getPosition(), player->getSize()), glm::ivec4(p->getPosition(), p->getSize()))) {
			if (!godMode) player->takeDamage(1);
			p->toRemove = true; // Marca la bala para ser borrada
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
	glm::mat4 view, model;

	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	
	
	view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.f));
	model = glm::mat4(1.0f);
	
	texProgram.setUniformMatrix4f("view", view);
	texProgram.setUniformMatrix4f("model", model);

	
	
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

	//Projectile projectile = new Projectile();

	{
		auto* p = new Projectile();
		// En coordenadas de mundo. Como la view usa -cameraPos, se ver� en la esquina superior-izquierda del mundo.
		p->init(glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f), texProgram);
		projectiles.push_back(p);
		p->render();
	}
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
	std::cout << "Todos los �tems a�adidos (placeholder)" << std::endl;
	// Aqu� puedes a�adir objetos al inventario si ya tienes esa mec�nica.
}

void Scene::teleportToInterior() {
	if (!map || !player) return;
	loadLevel("levels/interior.txt");

	//player->setPosition(glm::vec2(5 * map->getTileSize(), 5 * map->getTileSize()));
}

void Scene::teleportToBoss() {
	if (!map || !player) return;

	//player->setPosition(glm::vec2(40 * map->getTileSize(), 10 * map->getTileSize()));
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
	std::cout << "Todos los �tems a�adidos (placeholder)" << std::endl;
	// Aqu� puedes a�adir objetos al inventario si ya tienes esa mec�nica.
}

void Scene::teleportToInterior() {
	if (!map || !player) return;
	loadLevel("levels/interior.txt");

	//player->setPosition(glm::vec2(5 * map->getTileSize(), 5 * map->getTileSize()));
}

void Scene::teleportToBoss() {
	if (!map || !player) return;

	//player->setPosition(glm::vec2(40 * map->getTileSize(), 10 * map->getTileSize()));
}


