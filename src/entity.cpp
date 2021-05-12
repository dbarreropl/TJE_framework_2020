#include "entity.h"

Entity::Entity()
{

}

Entity::Entity(int id, bool visible)
{
	this->id = id;
	this->visible = visible;
}