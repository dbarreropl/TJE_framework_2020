#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"

class Player : public EntityMesh
{
public:

	Vector2 pos;
	bool moving = false;

	float playerVelocity = 15.0f;
	int animLenght = 4;
	int movementVelocity = 3.0f;

	Player(const char* mesh, const char* texture)
	: EntityMesh(mesh, texture){}

	void movePlayer(Vector3 move);

};

