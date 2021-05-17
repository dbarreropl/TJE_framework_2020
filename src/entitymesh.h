#pragma once
#include "utils.h"
#include "framework.h"
#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "animation.h"
#include "shader.h"
#include "scene.h"

class EntityMesh : public Entity
{
public:
	
	Mesh* mesh = NULL;
	Texture* texture = NULL;
	Animation* anim = NULL;
	Shader* shader;

	BoundingBox box_world;

	Vector4 color;
	enum EntityMeshType {
		DIRECTIONAL,
		POINT,
		SPOT		

	};
	EntityMeshType type;

	EntityMesh(const char* mesh, const char* texture);
	void render();
	//void update(float dt);
	Vector3 position_world() { return this->model * mesh->box.center; }
};

