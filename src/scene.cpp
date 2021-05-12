#include "scene.h"
#include <cassert>

Scene* Scene::instance = NULL;

Scene::Scene()
{
	assert(instance == NULL);

	instance = this;
	
}

void Scene::addEntity(Entity* Entity){
	
	Scene::instance->entities.push_back(Entity);
	if(Entity->type == 0)
		this->numPrefabs += 1;
	else if(Entity->type == 1)
		this->numCameras += 1;
	else if(Entity->type == 2)
		this->numLights += 1;
}