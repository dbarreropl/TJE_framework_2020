#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"

class Character : public EntityMesh
{
public:

	Animation walk;
	Animation shot;
	Animation* idle;

	float time_walk;
	float time_shot;

	bool isShooting = false;
	bool isMoving = false;
	bool isColiding = true;

	Vector3 targetMove;
	float movementVelocity = 3.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	Character(const char* mesh, const char* texture, const char* idle)
		: EntityMesh(mesh, texture) {
		
		this->setType(4);
		this->render_always = 1;
		this->idle = Animation::Get(idle);
		shot = Animation::Get("data/biglib/shooting.skanim");
		walk = Animation::Get("data/biglib/walking.skanim");
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	}

	void render();
	void movePlayer(Vector3 move);
	void onCollision(Entity* entity);

	void boundingSelected();

	void shoot();
};

