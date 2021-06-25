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

	Gui* def;
	Gui* text;
	Gui* text_done;

	Gui* mission_in;
	Gui* mission_done;
	Gui* mission_next;

	bool talked = FALSE;
	bool active=FALSE;
	bool done=FALSE;
	bool dead = FALSE;
	bool finish = FALSE;
	float time_dead;
	float mission;

	bool pos_dead = FALSE;

	Character(float mission, bool isNew, const char* mesh, const char* texture, const char* text, const char* text_done, const char* idle)
		: EntityMesh(mesh, texture) {
		
		this->setType(4);
		this->mission = mission;
		this->render_always = 1;
		this->idle = Animation::Get(idle);
		if(isNew)
			this->die = Animation::Get("data/biglib/die2.skanim");
		else
			this->die = Animation::Get("data/biglib/die.skanim");
		this->text = new Gui(text, false);
		this->text_done = new Gui(text_done, false);
		this->def= new Gui("data/gui/text.png", false);
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	}
	
	void render();
	void condition();
	void onTalk();

	void setMissionGui(const char* mission_in, const char* mission_next, const char* mission_done) {
		this->mission_in = new Gui(mission_in, false);
		this->mission_next = new Gui(mission_next, false);
		this->mission_done = new Gui(mission_done, false);
	}
};

