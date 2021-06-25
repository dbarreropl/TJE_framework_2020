#include "character.h"
#include "scene.h"
#include "game.h"
#include "input.h"

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
		}
		else {
			result = idle->skeleton;
			idle->assignTime(time);
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
		if (player->numberObjects("SM_Prop_Card") > 8)
			condition = true;
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

	this->done = condition;
	if (condition && !finish) {
		player->mission_gui = this->mission_done;
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
}
