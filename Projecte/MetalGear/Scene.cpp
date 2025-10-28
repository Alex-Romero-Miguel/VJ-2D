#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


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
	if(hud != NULL)
		delete hud; 
	
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
	hud = new HUD();
	hud->init(tile_size, glm::ivec2(SCREEN_X, SCREEN_Y), glm::ivec2(HUD_X_TILES * tile_size, map_size.y + HUD_Y_TILES * tile_size),player, texProgram);
	rations = Rations::createRations(&texProgram);
	rations->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 4) * tile_size, INIT_PLAYER_Y_TILES * tile_size));
	rations->setTileMap(map);
	projection = glm::ortho(0.f, float(map_size.x), float(map_size.y + (hud->getHeight() * tile_size)), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	hud->update(deltaTime);
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
	player->render();
	hud->render();
	rations->render();
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



