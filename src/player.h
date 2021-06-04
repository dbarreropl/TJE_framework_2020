#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"

class Player : public EntityMesh
{
public:

	Animation* walk;
	Animation* shot;
	float time_walk;

	bool isShooting = false;
	bool isMoving = false;
	bool isColiding = true;

	Vector3 targetMove;

	Vector3 velocity = Vector3(5.0f, 5.0f, 5.0f);
	float movementVelocity = 3.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	Player(const char* mesh, const char* texture)
		: EntityMesh(mesh, texture) {
	
		shot = Animation::Get("data/biglib/shooting.skanim");
		walk = Animation::Get("data/biglib/walking.skanim");
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	}

	void render();
	void movePlayer(Vector3 move);
	void updateCamera();
	void onCollision(Entity* entity);
	bool testCollision(EntityMesh* entity, Vector3 targetMove);

	void boundingSelected();

	void shoot();
};

