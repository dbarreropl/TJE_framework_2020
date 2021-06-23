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
	Animation* die;

	Gui* text;
	Gui* text_done;

	bool talked = FALSE;
	bool active=FALSE;
	bool done=FALSE;
	bool dead = FALSE;
	float time_dead;

	Character(const char* mesh, const char* text, const char* text_done, const char* idle)
		: EntityMesh(mesh, "data/biglib/WesternPack_renamed/texture.tga") {
		
		this->setType(4);
		this->render_always = 1;
		this->idle = Animation::Get(idle);
		this->die = Animation::Get("data/biglib/die.skanim");
		this->text = new Gui(text, false);
		this->text_done = new Gui(text_done, false);
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	}
	
	void render();
	bool condition();

};

