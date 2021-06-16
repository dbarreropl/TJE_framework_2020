#include "light.h"

Light::Light(float light_type, float intensity, Vector3 color, Vector3 position, Vector3 direction)
{
	this->type = LIGHT;
	this->light_type = (eLightType)light_type;
	this->intensity = intensity;
	this->color = color;
	this->position = position; //get in model
	this->direction = direction;
	this->directionWorld = Vector3(-this->position.x, -this->position.y, -this->position.z);
	this->model.setTranslation(position.x,position.y,position.z);

}

