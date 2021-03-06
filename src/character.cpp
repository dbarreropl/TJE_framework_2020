#include "character.h"
#include "scene.h"
#include "game.h"
#include "input.h"
#include "audio.h"

void Character::render()
{
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float time = Game::instance->time;

	shader->disable();
	shader->enable();
	if (shader && this->visible == TRUE)
	{
		Skeleton result;
		if (dead) {
			if ((time - time_dead) > die->duration-0.1) {
				die->assignTime(die->duration-0.1);
				time_dead = 0;
			}
			else
				die->assignTime(time - time_dead);

			result = die->skeleton;
			//die->assignTime(time);
			if (!pos_dead) {
				Vector3 newPos = model2.getTranslation()-model.getTranslation();
				Matrix44 T;
				T.setTranslation(newPos.x, 0, newPos.z);
				model = T*model;
				pos_dead = TRUE;
			}
		}
		else {
			result = idle->skeleton;
			idle->assignTime(time);
		}
		if (!dead) {
			if (!(this->name == "MexicanMale2"))
				model2 = model;
			else {
				//Matrix44 Spine = result.getBoneMatrix("mixamorig_Spine", false);
				model2 = result.getBoneMatrix("mixamorig_Spine", false) * model;
				Vector3 a = model2.getTranslation();
				//std::cout << a.x << std::endl;
				//std::cout << a.y << std::endl;
				//std::cout << a.z << std::endl;
			}
		}

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);
		
		//do the draw call
		mesh->renderAnimated(GL_TRIANGLES, &result);
	}
}

void Character::condition()
{
	Player* player = (Player*)Scene::instance->players[0];
	bool condition = false;

	if (this->name == "BusinessMan") {
		if (player->numberObjects("SM_Prop_Card") > 8) {
			condition = true;
			if (finish && player->nearCharacter==FALSE) {
				std::vector <Entity*> objects;
				for (int i = 0; i < player->objects.size(); i++) {
					if (player->objects[i]->name != "SM_Prop_Card")
						objects.push_back(player->objects[i]);
				}
				player->objects = objects;
			}
		}
		else
			condition = false;
	}

	if (this->name == "Sheriff" && this->active) {
		condition = true;
		for (int i = 0; i < Scene::instance->targets.size(); i++) { //targets
			if (Scene::instance->targets[i]->visible)
				condition = false;
		}
	}
	if (this->name == "GoldMiner") {
		if (player->bootlesBroke > 5)
			condition = true;
	}

	this->done = condition;
	if (condition && !finish) {
		player->mission_gui = this->mission_done;
		if (!sound_completed) {
			Audio::Play("data/audio/task_done.mp3", 1000, false);
			sound_completed = TRUE;
		}
	}

}

void Character::onTalk()
{
	Player* player = (Player*)Scene::instance->players[0];
	player->mission_gui = this->mission_in;
	if (this->name == "Sheriff") {
		for (int i = 0; i < Scene::instance->targets.size(); i++) { //targets
			Scene::instance->targets[i]->visible = TRUE;
		}
	}
	if (this->name == "GoldMiner") {
		player->bootlesBroke = 0;
	}
}
