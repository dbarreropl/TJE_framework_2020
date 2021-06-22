#include "character.h"
#include "scene.h"
#include "game.h"
#include "input.h"

void Character::render()
{
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float time = Game::instance->time;
	//shader = Shader::current;
	shader->disable();
	shader->enable();
	if (shader && this->visible == TRUE)
	{
		idle->assignTime(time);
		//result = idle.skeleton;
		//upload uniforms
		//shader->enable();
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);
		//do the draw call

		mesh->renderAnimated(GL_TRIANGLES, &idle->skeleton);
		//shader->disable();
	}
}
