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

void Character::movePlayer(Vector3 move) {

	if (isColiding == TRUE)
		this->model.translateGlobal(move.x, move.y, move.z);
	else
		this->model.translate(move.x, move.y, move.z);

}

void Character::onCollision(Entity* entity)
{
	if(entity->type==0)
		isColiding = TRUE;
}

void Character::boundingSelected()
{
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(screen_x, screen_y, Game::instance->window_width, Game::instance->window_height);
	for (int i = 0; i < Scene::instance->entities.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->entities[i];
		Vector3 col;
		Vector3 normal;
		if (current->mesh->testRayCollision(current->model, origin, dir, col, normal, 10)) {
			current->mesh->renderBounding(current->model);
			//current->visible = FALSE;
		}
	}
}

void Character::shoot()
{
	time_shot = Game::instance->time;
	isShooting = true;
	//entities colision
	std::vector <Entity*> entities;
	std::vector <Vector3> collisions;
	std::vector <Vector3> normals;
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;
	drawText(screen_x, screen_y, ".", Vector3(1, 1, 1), 4);
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(screen_x, screen_y, Game::instance->window_width, Game::instance->window_height);
	for (int i = 0; i < Scene::instance->entities.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->entities[i];
		Vector3 col;
		Vector3 normal;
		if (current->mesh->testRayCollision(current->model, origin, dir, col, normal, 10)) {
			entities.push_back(current);
			collisions.push_back(col);
			normals.push_back(normal);
		}
	}

	//near entity
	Vector3 pos = this->model.getTranslation();
	Entity* entity = NULL;
	Vector3 entity_col = Vector3(0,0,0);
	Vector3 entity_normal = Vector3(0, 0, 0);
	float minDist = 999;
	for (int i = 0; i < entities.size(); i++) {
		float dist = entities[i]->model.getTranslation().distance(pos);
		if (dist < minDist) {
			entity = entities[i];
			entity_col = collisions[i];
			entity_normal = normals[i];
			minDist = dist;
		}
	}

	//create bullet hole
	if (entity) {
		//entity->visible = FALSE;
		Mesh* mesh = Mesh::Get("data/bullet.obj");
		Texture* texture = Texture::Get("data/bullet2.png");
		Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		EntityMesh* entity_sh = new EntityMesh();
		entity_sh->mesh = mesh;
		entity_sh->texture = texture;
		entity_sh->shader = shader;
		entity_sh->render_always = TRUE;
		entity_sh->visible = TRUE;
	
		//hole near player
		if (pos.x > entity_col.x)
			entity_col.x = entity_col.x + 0.01;
		else
			entity_col.x = entity_col.x - 0.01;
		if (pos.y > entity_col.y)
			entity_col.y = entity_col.y + 0.01;
		else
			entity_col.y = entity_col.y - 0.01;
		if (pos.z > entity_col.z)
			entity_col.z = entity_col.z + 0.01;
		else
			entity_col.z = entity_col.z - 0.01;

		//error normalize lenght
		if (abs(entity_normal.x) < 0.000001)
			entity_normal.x = 0.000001;
		if (abs(entity_normal.y) < 0.000001)
			entity_normal.y = 0.000001;
		if (abs(entity_normal.z) < 0.000001)
			entity_normal.z = 0.000001;
	
		entity_sh->model.setTranslation(entity_col.x, entity_col.y, entity_col.z);
		entity_sh->model.setFrontAndOrthonormalize(entity_normal);
		Scene::instance->addBulletHole(entity_sh);
	}
}
