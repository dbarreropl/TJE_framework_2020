#pragma once
#include "utils.h"
#include "framework.h"
#include "entitymesh.h"
#include "entity.h"
#include "gui.h"

class Character : public EntityMesh
{
public:

	Animation* idle;

	Gui* text;
	bool active;

	Character(const char* mesh, const char* gui, const char* idle)
		: EntityMesh(mesh, "data/biglib/WesternPack_renamed/texture.tga") {
		
		this->setType(4);
		this->render_always = 1;
		this->idle = Animation::Get(idle);
		this->text = new Gui(gui, false);
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	}
	
	void render();

};

