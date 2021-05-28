#include "entity.h"

Entity::Entity()
{
	this->visible = TRUE;
}

Entity::Entity(int id, bool visible)
{
	this->id = id;
	this->visible = visible;
}