#include "gui.h"
#include "scene.h"
#include "game.h"
#include "input.h"

Gui::Gui(const char* texture, bool invert)
{
	this->invert = invert;
	hover = false;
	cam2D = new Camera();
	this->setType(5);
	this->texture = Texture::Get(texture);
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");

}

void Gui::render(float x, float y,float w,float h, bool can_hover) {

	shader->enable();
	//shader = Shader::current;
	if (shader && this->visible==TRUE)
	{
		//hover
		if (can_hover) {
			Vector2 mouse = Input::mouse_position;
			float halfWidth = w * 0.45;
			float halfHeight = h * 0.45;
			float min_x = x - halfWidth;
			float min_y = y - halfHeight;
			float max_x = x + halfWidth;
			float max_y = y + halfHeight;

			hover = mouse.x > min_x && mouse.y > min_y && mouse.x < max_x && mouse.y < max_y;
			if (hover) {
				w = w * 1.2;
				h = h * 1.2;
			}
		}

		cam2D->setOrthographic(0,Game::instance->window_width, Game::instance->window_height,0,-1,1);
		cam2D->enable();
		quad.createQuad(x,y,w,h,invert);
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam2D->viewprojection_matrix);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time);
		
		//do the draw call
		quad.render(GL_TRIANGLES);
		
		//shader->disable();
	}
}