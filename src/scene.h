#pragma once
#include "utils.h"
#include "framework.h"
#include "entity.h"
#include "light.h"
#include "entitymesh.h"
#include "camera.h"


class Scene
{
public:

	static Scene* instance;
	std::vector <Entity*> entities;

	
	int numPrefabs = 0;
	int numCameras = 0;
	int numLights = 0;
	bool loaded = FALSE;

	bool mode=0;

	//Shader* current_shader = NULL;

	Scene();
public:
	void addEntity(Entity* Entity);
	void loadScene();
};
