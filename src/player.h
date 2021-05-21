#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"

class Player : public EntityMesh
{
public:

	Vector2 pos;
	bool can_move = true;

	float playerVelocity = 15.0f;
	int animLenght = 4;
	int movementVelocity = 3.0f;
	float yaw=0.0f;
	float pitch=0.0f;

	Player(const char* mesh, const char* texture)
	: EntityMesh(mesh, texture){}

	void movePlayer(Vector3 move);
	void updateCamera();
	void onCollision(Entity* entity);
	bool testCollision(EntityMesh* entity);
};

