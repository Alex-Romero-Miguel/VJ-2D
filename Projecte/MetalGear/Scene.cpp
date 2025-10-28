#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Guard.h"
#include "Dog.h"
#include "Camera.h"

#include <fstream>
#include <sstream>s

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 15
#define INIT_PLAYER_Y_TILES 15

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
	currentLevel = 1;
	initShaders();

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	cameraPos = glm::vec2(0,0);

	//map = TileMap::createTileMap("levels/exterior.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	loadLevel("levels/exterior.txt");
	playerStartPos = glm::ivec2(15, 15);

	//loadLevel("levels/interior.txt", glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES));

	/*
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
	

	projection = glm::ortho(0.f, float(SCREEN_WIDTH+ SCREEN_X), float(SCREEN_HEIGHT+ SCREEN_Y), 0.f);
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


	////  Cargar los enemigos del nuevo nivel (¡esto debería leerse del archivo del nivel!)
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

	// Busca la sección [ENEMIES] en el archivo
	while (getline(fin, line) && line.find("ENEMIES") == string::npos) {
		// Sigue leyendo hasta encontrar la sección o el final del archivo
	}

	// Si se encontró, empieza a leer los datos de los enemigos
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

			// Solo haz la configuración específica de la cámara aquí
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

		// Si se creó un enemigo válido, inicialízalo y añádelo a la lista
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
	// Comprobamos si el jugador está muerto
	if (player->isDead()) {
		deathTimer -= deltaTime;
		if (deathTimer <= 0)
			restartGame();
		return;
	}

	glm::vec2 playerPos = player->getPosition();

	// Si el jugador se sale por la derecha
	if (playerPos.x + player->getSize().x > cameraPos.x + SCREEN_WIDTH) {
		cameraPos.x +=SCREEN_WIDTH; // Mueve la cámara una pantalla a la derecha
		player->setPosition(glm::vec2(playerPos.x + player->getSize().x + 1, playerPos.y)); // Coloca al jugador al inicio de la nueva pantalla
	}
	// Si el jugador se sale por la izquierda
	else if (playerPos.x < cameraPos.x ) {
		cameraPos.x -=SCREEN_WIDTH; // Mueve la cámara a la izquierda
		player->setPosition(glm::vec2(playerPos.x - player->getSize().x - 1, playerPos.y)); // Coloca al jugador al final de la nueva pantalla
	}
	// Si el jugador se sale por abajo
	else if (playerPos.y + player->getSize().y > cameraPos.y + SCREEN_HEIGHT) {
		cameraPos.y += SCREEN_HEIGHT; // Mueve la cámara una pantalla hacia abajo
		player->setPosition(glm::vec2(playerPos.x, playerPos.y + player->getSize().y + 1)); // Coloca al jugador al inicio de la nueva pantalla
	}
	// Si el jugador se sale por arriba
	else if (playerPos.y < cameraPos.y) {
		cameraPos.y -= SCREEN_HEIGHT; // Mueve la cámara una pantalla hacia arriba
		player->setPosition(glm::vec2(playerPos.x, playerPos.y - player->getSize().y - 1));
	}

	// Actualizamos al jugador
	player->update(deltaTime);

	for (auto it = enemies.begin(); it != enemies.end(); /* El incremento se hace dentro */)
	{
		Enemy* enemy = *it;

		// 1. Comprueba si el enemigo está en la pantalla actual
		bool isOnScreen = (enemy->getPosition().x + enemy->getSize().x > cameraPos.x &&
			enemy->getPosition().x < cameraPos.x + 256.f &&
			enemy->getPosition().y + enemy->getSize().y > cameraPos.y &&
			enemy->getPosition().y < cameraPos.y + 192.f);

		// 2. Actualiza la IA solo si está activo
		if (isOnScreen) {
			enemy->update(deltaTime);
		}
		else {
			enemy->resetState();
		}

		// 3. Comprueba el ataque del jugador (puñetazo)
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
			it = enemies.erase(it); // Si se borra, el iterador avanza automáticamente
		}
		else {
			++it; // Si no se borra, avanza el iterador manualmente
		}
	}

	//for (auto it = enemies.begin(); it != enemies.end();) {
	//	Enemy* enemy = *it;

	//	// --- COMPROBACIÓN DE ACTIVACIÓN ---
	//	// Comprueba si el enemigo está en la pantalla actual
	//	bool isOnScreen = (enemy->getPosition().x + enemy->getSize().x > cameraPos.x &&
	//		enemy->getPosition().x < cameraPos.x + 256.f &&
	//		enemy->getPosition().y + enemy->getSize().y > cameraPos.y &&
	//		enemy->getPosition().y < cameraPos.y + 192.f);

	//	// Si el enemigo está activo (en pantalla o hay alerta global), actualízalo.
	//	/*if (isOnScreen || isGlobalAlert) {*/
	//	if (isOnScreen){
	//		enemy->update(deltaTime); // Ahora la llamada a update es más simple
	//	}
	//	else {
	//		enemy->resetState();
	//	}
		//else {
		//	//enemy->(); // Asegúrate de que tenga una animación de "quieto"
		//}
		// --- FIN DE LA COMPROBACIÓN ---


		//// Comprobamos colisión de ataque o contacto (esta lógica puede ir dentro del if de arriba si prefieres)
		//if (!enemy->isDead() && checkCollision(/*...*/))
		//{
		//	if (enemy->attack(deltaTime) && !godMode)
		//		player->takeDamage(enemy->getDamage());
		//}

		// Eliminamos enemigos muertos
		/*if (enemy->toRemove) {
			delete enemy;
			it = enemies.erase(it);
		}
		else {
			++it;
		}*/
	
	// Golpe del jugador 
	//glm::ivec4 hitbox = player->getPunchHitbox();
	//if (hitbox.z > 0 && hitbox.w > 0) {
	//	for (Enemy* enemy : enemies) {
	//		if (enemy->isDead()) continue;

	//		glm::vec2 epos = enemy->getPosition();
	//		glm::ivec2 esize = enemy->getSize();
	//		if (checkCollision(hitbox, glm::ivec4(epos.x, epos.y, esize.x, esize.y))) {
	//			std::cout << "Golpeando enemigo en " << typeid(*enemy).name() << std::endl;
	//			enemy->takeDamage(1);
	//		}
	//	}
	//}

	//// Actualizamos enemigos y comprobamos ataques
	//for (auto it = enemies.begin(); it != enemies.end();) {
	//	Enemy* enemy = *it;
	//	//enemy->update(deltaTime);

	//	// Comprobamos colisión de ataque o contacto
	//	if (!enemy->isDead() && checkCollision(
	//		glm::ivec4(player->getPosition().x, player->getPosition().y, player->getSize().x, player->getSize().y),
	//		glm::ivec4(enemy->getPosition().x, enemy->getPosition().y, enemy->getSize().x, enemy->getSize().y)))
	//	{
	//		// Si hay contacto y el enemigo ataca, y no hay godMode...
	//		if (enemy->attack(deltaTime) && !godMode)
	//			player->takeDamage(enemy->getDamage());
	//	}

	//	// Eliminamos enemigos muertos (tras su animación, si tienen)
	//	if (enemy->toRemove) {
	//		delete enemy;
	//		it = enemies.erase(it);
	//	}
	//	else {
	//		++it;
	//	}
	//}
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
	loadLevel("levels/interior.txt");

	//player->setPosition(glm::vec2(5 * map->getTileSize(), 5 * map->getTileSize()));
}

void Scene::teleportToBoss() {
	if (!map || !player) return;

	//player->setPosition(glm::vec2(40 * map->getTileSize(), 10 * map->getTileSize()));
}


