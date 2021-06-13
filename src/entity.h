#pragma once
#include "utils.h"
#include "framework.h"

class Entity
{
public:
	unsigned int id;
	Matrix44 model;
	
	bool visible;
	bool render_always=0;
	enum eType {
		PREFAB,
		CAMERA,
		LIGHT,
		PLAYER,
		ENEMIE

	};
	eType type;

	Entity();
	Entity(int id, bool visible );
	//virtual ~Entity(); //destructor

	void setType(float type) { this->type = (eType)type; }
	virtual void render() {}

	//virtual void update(float elapsed_time);
	//Vector3 getPosition();
};

