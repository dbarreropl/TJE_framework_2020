#include "scene.h"
#include <cassert>

Scene* Scene::instance = NULL;

Scene::Scene()
{
	assert(instance == NULL);
	instance = this;
}

void Scene::initGame()
{
	Player* player = (Player*)Scene::instance->players[0];
	player->model.setTranslation(player->initialPos.x, player->initialPos.y, player->initialPos.z);
	player->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	player->bullets = 6;
	for (int i = 0; i < Scene::instance->characters.size(); i++) {
		Character* current = (Character*)Scene::instance->characters[i];
		current->dead = FALSE;
	}
	for (int i = 0; i < Scene::instance->entities.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->entities[i];
		if(current->name=="Target")
			current->visible = TRUE;
		else
			current->visible = TRUE;
	}

	//bullet holes
	Scene::instance->bullet_holes.clear();

	//objects
	player->objects.clear();
	for (int i = 0; i < Scene::instance->guis.size(); i++) {
		Gui* object = (Gui*)Scene::instance->guis[i];
		object->number = 1;
	}

	//missions
	player->mission = 1;
	player->mission_gui = new Gui("data/gui/first_mission.png", false);

	for (int i = 0; i < Scene::instance->characters.size(); i++) {
		Character* character = (Character*)Scene::instance->characters[i];
		character->talked = FALSE;
		character->active = FALSE;
		character->done = FALSE;
		character->dead = FALSE;
		character->finish = FALSE;
	}

	init = TRUE;
}
void Scene::addEntity(Entity* Entity){
	
	if (Entity->type == 0) {
		Scene::instance->entities.push_back(Entity);
	}
	else if (Entity->type == 1) {
		Scene::instance->cameras.push_back(Entity);
	}
	else if (Entity->type == 2) {
		Scene::instance->lights.push_back(Entity);
	}
	else if (Entity->type == 3) {
		Scene::instance->players.push_back(Entity);
	}
	else if (Entity->type == 4) {
		Scene::instance->characters.push_back(Entity);
	}
	else if (Entity->type == 5) {
		Scene::instance->guis.push_back(Entity);
	}
	else{
		Scene::instance->entities.push_back(Entity);
	}
}

void Scene::addBulletHole(Entity* Entity) {
	Scene::instance->bullet_holes.push_back(Entity);
}

void Scene::addTarget(Entity* Entity) {
	Scene::instance->targets.push_back(Entity);
}

void Scene::loadCharacters()
{
	//characters
	Entity* BusinessMan = new Character(2, 0, "data/biglib/Characters/Business_Man.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/BusinessMan.png", "data/gui/BusinessMan_done.png", "data/biglib/old_man_idle.skanim");
	BusinessMan->model.setTranslation(4.700, 0, 4.400);
	BusinessMan->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	BusinessMan->name = "BusinessMan";
	Scene::instance->addEntity(BusinessMan);
	Character* BusinessMan_character = (Character*)Scene::instance->characters.back();
	BusinessMan_character->setMissionGui("data/gui/BusinessMan_mission_in.png", "data/gui/BusinessMan_mission_next.png", "data/gui/BusinessMan_mission_done.png");

	Entity* CowBoy = new Character(8, 0, "data/biglib/Characters/CowBoy.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/arm_stretching.skanim");
	CowBoy->model.setTranslation(12.800, 0, -12.4);
	CowBoy->model.rotate(DEG2RAD * 90.f, Vector3(0.0f, 1.0f, 0.0f));
	CowBoy->name = "CowBoy";
	Scene::instance->addEntity(CowBoy);

	Entity* CowGirl = new Character(8, 0, "data/biglib/Characters/CowGirl.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/briefcase_idle.skanim");
	CowGirl->model.setTranslation(-2.2, 0, -13.9);
	CowGirl->name = "CowGirl";
	Scene::instance->addEntity(CowGirl);

	Entity* GunMan = new Character(8, 0, "data/biglib/Characters/GunMan.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/sitting.skanim");
	GunMan->model.setTranslation(8.7, 0.2, -19.4);
	GunMan->name = "GunMan";
	Scene::instance->addEntity(GunMan);

	Entity* Sheriff = new Character(1, 0, "data/biglib/Characters/Sheriff.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/Sheriff.png", "data/gui/Sheriff_done.png", "data/biglib/drunk_idle.skanim");
	Sheriff->model.setTranslation(3.8, 0, -37.9);
	Sheriff->model.rotate(DEG2RAD * 90.f, Vector3(0.0f, 1.0f, 0.0f));
	Sheriff->name = "Sheriff";
	Scene::instance->addEntity(Sheriff);
	Character* Sheriff_character = (Character*)Scene::instance->characters.back();
	Sheriff_character->setMissionGui("data/gui/Sheriff_mission_in.png", "data/gui/Sheriff_mission_next.png", "data/gui/Sheriff_mission_done.png");

	Entity* Woman = new Character(8, 0, "data/biglib/Characters/Woman.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle.skanim");
	Woman->model.setTranslation(-9.700, 0, -38.49);
	Woman->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	Woman->name = "Woman";
	Scene::instance->addEntity(Woman);

	Entity* WorkingGirl = new Character(8, 0, "data/biglib/Characters/WorkingGirl.mesh", "data/biglib/WesternPack_renamed/texture.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/jazz_dancing.skanim");
	WorkingGirl->model.setTranslation(-17.2, 0.5, -13.4);
	WorkingGirl->model.rotate(DEG2RAD * 270.f, Vector3(0.0f, 1.0f, 0.0f));
	WorkingGirl->name = "WorkingGirl";
	Scene::instance->addEntity(WorkingGirl);

	Entity* GoldMiner = new Character(3, 1, "data/biglib/NewCharacters/GoldMiner.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/GoldMiner.png", "data/gui/GoldMiner_done.png", "data/biglib/breathing_idle2.skanim");
	GoldMiner->model.setTranslation(-13, 0.5, -16);
	GoldMiner->model.scale(0.01, 0.01, 0.01);
	GoldMiner->name = "GoldMiner";
	Scene::instance->addEntity(GoldMiner); //7
	Character* GoldMiner_character = (Character*)Scene::instance->characters.back();
	GoldMiner_character->setMissionGui("data/gui/GoldMiner_mission_in.png", "data/gui/GoldMiner_mission_next.png", "data/gui/GoldMiner_mission_done.png");

	Entity* GoldMiner2 = new Character(8, 1, "data/biglib/NewCharacters/GoldMiner2.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	GoldMiner2->model.setTranslation(-13, 0, -28.5);
	GoldMiner2->model.rotate(90 * DEG2RAD, Vector3(0, -1, 0));
	GoldMiner2->model.scale(0.01, 0.01, 0.01);
	GoldMiner2->name = "GoldMiner2";
	Scene::instance->addEntity(GoldMiner2);

	Entity* Hunter = new Character(8, 1, "data/biglib/NewCharacters/Hunter.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	Hunter->model.setTranslation(-29, 0, -2.5);
	Hunter->model.scale(0.01, 0.01, 0.01);
	Hunter->name = "Hunter";
	Scene::instance->addEntity(Hunter);

	Entity* MexicanFemale = new Character(8, 1, "data/biglib/NewCharacters/MexicanFemale.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	MexicanFemale->model.setTranslation(-11, 0, -51);
	MexicanFemale->model.rotate(90 * DEG2RAD, Vector3(0, -1, 0));
	MexicanFemale->model.scale(0.01, 0.01, 0.01);
	MexicanFemale->name = "MexicanFemale";
	Scene::instance->addEntity(MexicanFemale);

	Entity* Traveller = new Character(8, 1, "data/biglib/NewCharacters/Traveller.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	Traveller->model.setTranslation(6, 3, -10.5);
	Traveller->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	Traveller->model.scale(0.01, 0.01, 0.01);
	Traveller->name = "Traveller";
	Scene::instance->addEntity(Traveller);

	Entity* MexicanMale = new Character(8, 1, "data/biglib/NewCharacters/MexicanMale.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/walk_in_circle2.skanim");
	MexicanMale->model.setTranslation(0, 0, -24);
	MexicanMale->model.scale(0.01, 0.01, 0.01);
	MexicanMale->name = "MexicanMale";
	Scene::instance->addEntity(MexicanMale);

	Entity* MexicanMale2 = new Character(8, 1, "data/biglib/NewCharacters/MexicanMale2.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/walk_in_circle2.skanim");
	MexicanMale2->model.setTranslation(15, 0, 0);
	MexicanMale2->model.scale(0.01, 0.01, 0.01);
	MexicanMale2->name = "MexicanMale2";
	Scene::instance->addEntity(MexicanMale2);

	Entity* PriestMan = new Character(8, 1, "data/biglib/NewCharacters/PriestMan.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	PriestMan->model.setTranslation(0.5, 3, -72);
	PriestMan->model.scale(0.01, 0.01, 0.01);
	PriestMan->name = "PriestMan";
	Scene::instance->addEntity(PriestMan);

	Entity* SalesMan = new Character(8, 1, "data/biglib/NewCharacters/SalesMan.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle2.skanim");
	SalesMan->model.setTranslation(15, 0, -45);
	Traveller->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
	SalesMan->model.scale(0.01, 0.01, 0.01);
	SalesMan->name = "SalesMan";
	Scene::instance->addEntity(SalesMan);

	Entity* Thug = new Character(8, 1, "data/biglib/NewCharacters/Thug.mesh", "data/biglib/WesternPack_renamed/texture4.tga", "data/gui/text.png", "data/gui/text.png", "data/biglib/defeated.skanim");
	Thug->model.setTranslation(9.5, 0, -38.5);
	Thug->model.scale(0.01, 0.01, 0.01);
	Thug->name = "Thug";
	Scene::instance->addEntity(Thug);

}

void Scene::loadGui()
{
	//Gui
	Entity* title = new Gui("data/gui/title.png", false);
	Scene::instance->addEntity(title);

	Entity* title2 = new Gui("data/gui/main_play.png", false);
	Scene::instance->addEntity(title2);

	Entity* paper = new Gui("data/gui/paper.png", true);
	Scene::instance->addEntity(paper);

	Entity* resume = new Gui("data/gui/resume.png", false);
	Scene::instance->addEntity(resume);

	Entity* exit = new Gui("data/gui/exit.png", false);
	Scene::instance->addEntity(exit);

	Entity* loading = new Gui("data/gui/loading2.png", false); //5
	Scene::instance->addEntity(loading);

	Entity* text = new Gui("data/gui/text.png", false);
	Scene::instance->addEntity(text);

	Entity* talk_key = new Gui("data/gui/talk.png", false);
	Scene::instance->addEntity(talk_key);

	Entity* title3 = new Gui("data/gui/main_exit.png", false);
	Scene::instance->addEntity(title3);

	Entity* pick_up = new Gui("data/gui/pick_up.png", false);
	Scene::instance->addEntity(pick_up);

	Entity* scope = new Gui("data/gui/scope.png", false); //10
	Scene::instance->addEntity(scope);

	Entity* scope_shoot = new Gui("data/gui/scope_shoot.png", false);
	Scene::instance->addEntity(scope_shoot);

	Entity* bullet_0 = new Gui("data/gui/bullet_0.png", false);
	Scene::instance->addEntity(bullet_0);

	Entity* bullet_1 = new Gui("data/gui/bullet_1.png", false);
	Scene::instance->addEntity(bullet_1);

	Entity* bullet_2 = new Gui("data/gui/bullet_2.png", false);
	Scene::instance->addEntity(bullet_2);

	Entity* bullet_3 = new Gui("data/gui/bullet_3.png", false); //15
	Scene::instance->addEntity(bullet_3);

	Entity* bullet_4 = new Gui("data/gui/bullet_4.png", false);
	Scene::instance->addEntity(bullet_4);

	Entity* bullet_5 = new Gui("data/gui/bullet_5.png", false);
	Scene::instance->addEntity(bullet_5);

	Entity* bullet_6 = new Gui("data/gui/bullet_6.png", false);
	Scene::instance->addEntity(bullet_6);

	Entity* card = new Gui("data/gui/card.png", false);
	Scene::instance->addEntity(card);

	Entity* object_1 = new Gui("data/gui/object_1.png", false); //20
	Scene::instance->addEntity(object_1);

	Entity* object_2 = new Gui("data/gui/object_2.png", false);
	Scene::instance->addEntity(object_2);

	Entity* object_3 = new Gui("data/gui/object_3.png", false);
	Scene::instance->addEntity(object_3);

	Entity* object_4 = new Gui("data/gui/object_4.png", false);
	Scene::instance->addEntity(object_4);

	Entity* object_5 = new Gui("data/gui/object_5.png", false);
	Scene::instance->addEntity(object_5);

	Entity* object_6 = new Gui("data/gui/object_6.png", false); //25
	Scene::instance->addEntity(object_6);

	Entity* object_7 = new Gui("data/gui/object_7.png", false);
	Scene::instance->addEntity(object_7);

	Entity* object_8 = new Gui("data/gui/object_8.png", false);
	Scene::instance->addEntity(object_8);

	Entity* object_9 = new Gui("data/gui/object_9.png", false);
	Scene::instance->addEntity(object_9);

	Entity* object_9p = new Gui("data/gui/object_9+.png", false);
	Scene::instance->addEntity(object_9p);

	Entity* gameover = new Gui("data/gui/gameover.png", false); //30
	Scene::instance->addEntity(gameover);

	Entity* won = new Gui("data/gui/won.png", false);
	Scene::instance->addEntity(won);

	Entity* close = new Gui("data/gui/close.png", false);
	Scene::instance->addEntity(close);

	Entity* tutorial = new Gui("data/gui/tutorial.png", false);
	Scene::instance->addEntity(tutorial);
}

void Scene::loadTargets()
{
	//target
	Entity* target = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target->model.setTranslation(-17.4, 1.6, -7.4);
	Scene::instance->addTarget(target);

	Entity* target2 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target2->model.setTranslation(15.1, 4.1, -12.9);
	target2->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target2);

	Entity* target3 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target3->model.setTranslation(15.6, 3.4, -35.5);
	target3->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target3);

	Entity* target4 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target4->model.setTranslation(-8.1, 2.7, -30);
	Scene::instance->addTarget(target4);

	Entity* target5 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target5->model.setTranslation(7.4, 4.5, 10.7);
	target5->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target5);

	for (int i = 0; i < Scene::instance->targets.size(); i++) { //targets
		Scene::instance->targets[i]->name = "Target";
		Scene::instance->targets[i]->visible = FALSE;
	}
}



void Scene::loadScene() {
	loadCharacters();
	loadGui();
	loadTargets();

	//sky
	Entity* sky = new EntityMesh("data/skydome.obj", "data/skydome.png");
	sky->model.scale(24.973, 24.973, 24.973);
	sky->render_always = 1;
	sky->setType(0);
	Scene::instance->addEntity(sky);

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

		//if (!data_spl[0].find("SM_Prop_Coffin_01")) {

		//entity->model.rotate(DEG2RAD * (std::stof(rot[2])), Vector3(0.0f, 0.0f, 1.0f)); //1 where rotation applies //(std::stof(rot[0])/4)
		entity->model.rotate(DEG2RAD * (std::stof(rot[1])), Vector3(0.0f, 1.0f, 0.0f));
		entity->model.rotate(DEG2RAD * (std::stof(rot[0])), Vector3(-1.0f, 0.0f, 0.0f));

		//scale
		std::vector <std::string> scl = split(data_spl[3], ',');
		entity->model.scale(std::stof(scl[0]), std::stof(scl[1]), std::stof(scl[2]));

		//type
		entity->setType(0);

		//name
		entity->name = data_spl[0].c_str();
		Scene::instance->addEntity(entity);
		if (!data_spl[0].find("SM_Prop_Card")) {
			entity->can_pickUp = true;
			entity->name = "SM_Prop_Card";
		}

		if (!data_spl[0].find("SM_Prop_Bottle")) {
			entity->name = "Bottle";
		}
	}
	this->loaded = TRUE;

}