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

void Scene::loadScene() {
	
	//load file scene
	std::string data;
	std::string line;
	readFile("data/data.txt", data);
	std::vector <std::string> data_lines = split(data, '\n');

	for (int i = 0; i < data_lines.size(); i++) {
		data_lines[i].pop_back();
		std::vector <std::string> data_spl = split(data_lines[i], ' ');

		//create entity
		Entity* entity;
		if (!data_spl[0].find("SM_Bld_Sign"))
			entity = new EntityMesh(("data/biglib/WesternPack_renamed/All/"+data_spl[0]).c_str(),"data/biglib/WesternPack_renamed/texture2.tga");
		else if(!data_spl[0].find("SM_Env_BackgroundCard"))
			entity = new EntityMesh(("data/biglib/WesternPack_renamed/All/" + data_spl[0]).c_str(), "data/biglib/WesternPack_renamed/texture3.tga");
		else
			entity = new EntityMesh(("data/biglib/WesternPack_renamed/All/" + data_spl[0]).c_str(), "data/biglib/WesternPack_renamed/texture.tga");

		//position
		std::vector <std::string> pos = split(data_spl[1], ',');
		entity->model.setTranslation(-std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));

		//rotation
		std::vector <std::string> rot = split(data_spl[2], ',');
		entity->model.rotate(DEG2RAD * (std::stof(rot[0])), Vector3(1.0f, 0.0f, 0.0f)); //1 where rotation applies
		entity->model.rotate(DEG2RAD * (std::stof(rot[1])), Vector3(0.0f, 1.0f, 0.0f));
		entity->model.rotate(DEG2RAD * (std::stof(rot[2])), Vector3(0.0f, 0.0f, 1.0f));

		//scale
		std::vector <std::string> scl = split(data_spl[3], ',');
		entity->model.scale(std::stof(scl[0]), std::stof(scl[1]), std::stof(scl[2]));

		Scene::instance->addEntity(entity);
	}
	this->loaded = TRUE;
		
}