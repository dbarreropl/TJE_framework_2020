#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"
#include "gui.h"

class Player : public EntityMesh
{
public:
	float x = 0, y = 0, z = 0;
	Vector3 initialPos;
	Vector3 targetMove;
	std::vector <Entity*> objects;

	Gui* mission_gui;

	Animation* walk;
	Animation* shot;
	Animation* reload;
	float time_walk;
	float time_shot;
	float time_reload;
	float bullets = 6;

	float height_floor=0.1f;
	bool aux;
	EntityMesh gun;
	bool isShooting = false;
	bool isReloading = false;
	bool isMoving = false;
	bool isColiding = true;
	bool isTalking = false;

	bool canShoot = true;
	bool nearCharacter = false;
	bool nearObject = false;
	float mission = 1;
	bool game_over=false;
	bool won=false;

	Vector3 velocity = Vector3(5.0f, 5.0f, 5.0f);
	float movementVelocity = 3.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	Player(const char* mesh, const char* texture)
		: EntityMesh(mesh, texture) {
	
		this->render_always = 1;
		this->setType(3);
		gun = EntityMesh("data/biglib/WesternPack_renamed/All/revolver_01.obj", texture);
		Vector3 pos = Vector3(4.700, 0, 4.400);
		gun.model.setTranslation(pos.x,pos.y+1.5,pos.z+0.9);
		gun.model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
		shot = Animation::Get("data/biglib/shooting.skanim");
		reload = Animation::Get("data/biglib/reload.skanim");
		walk = Animation::Get("data/biglib/walking.skanim");
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");

		mission_gui = new Gui("data/gui/first_mission.png", false);
	}

	void render();
	void movePlayer(Vector3 move);
	void updateCamera();
	void onCollision(Entity* entity);
	bool testCollision(EntityMesh* entity, Vector3 targetMove);

	void pickUp();
	void addObject(Entity* entity);
	bool hasObject(std::string object);
	float numberObjects(std::string object);

	void shoot();
};

