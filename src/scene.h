#pragma once
#include "utils.h"
#include "framework.h"
#include "entity.h"
#include "light.h"
#include "entitymesh.h"
#include "camera.h"
#include "player.h"
#include "character.h"
#include "gui.h"

class Scene
{
public:

	static Scene* instance;
	std::vector <Entity*> entities;
	std::vector <Entity*> players;
	std::vector <Entity*> characters;
	std::vector <Entity*> cameras;
	std::vector <Entity*> lights;
	std::vector <Entity*> guis;
	std::vector <Entity*> bullet_holes;
	std::vector <Entity*> targets;
	
	bool loaded = FALSE;
	bool init = false;

	bool mode=1; //mode edit=0, mode play=1

	Scene();
public:
	void addEntity(Entity* Entity);
	void addBulletHole(Entity* Entity);
	void addTarget(Entity* Entity);

	void initGame();
	
	void loadCharacters();
	void loadGui();
	void loadTargets();
	void loadScene();
};
