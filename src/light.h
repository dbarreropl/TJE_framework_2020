#pragma once
#include "utils.h"
#include "framework.h"
#include "entity.h"
#include "camera.h"

class Light : public Entity
{
public:
	
	Vector3 color;
	Vector3 position;
	Vector3 direction;
	Vector3 directionWorld;
	float intensity;
	float max_distance;

	Camera* camera;

	enum eLightType {
		DIRECTIONAL,
		POINT,
		SPOT		

	};
	eLightType light_type;

	Light(float light_type, float intensity, Vector3 color, Vector3 position, Vector3 direction);
	void render(){}
};

