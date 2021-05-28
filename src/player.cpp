#include "player.h"
#include "scene.h"
#include "game.h"
#include "input.h"

void Player::movePlayer(Vector3 move) {
	this->model.translate(move.x, move.y, move.z);
	
}

void Player::updateCamera()
{
	Camera* cam = (Camera*)Scene::instance->cameras[0];
	Vector3 player_pos = this->model.getTranslation();
	float elapsed_time = Game::instance->elapsed_time;
	//cam->lookAt(Vector3(player_pos.x+0.2, player_pos.y + 2.5, player_pos.z + 2.5), Vector3(player_pos.x + 0.3, player_pos.y + 1.6, player_pos.z - 2.5), Vector3(0.f, 1.f, 0.f));

	Matrix44 pitch;
	//this->model.rotate(this->pitch * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
	pitch.rotate(this->pitch * DEG2RAD, Vector3(1.0f,0.0f,0.0f));
	Vector3 forward = pitch.rotateVector(Vector3(0.0f, 0.0f, 1.0f));
	forward = this->model.rotateVector(forward);

	Vector3 eye = this->model * Vector3(0.0f, 1.5f, 0.5f);
	Vector3 center = eye + forward;

	Vector3 up = Vector3(0.0f,1.0f,0.0f);
	cam->lookAt(eye,center,up);
	cam->rotate(this->yaw, Vector3(0.0f, -1.0f, 0.0f));
	cam->rotate(this->pitch, cam->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
}

void Player::onCollision(Entity* entity)
{
	if(entity->type==0)
		can_move = false;
}

bool Player::testCollision(EntityMesh* entity, Vector3 targetMove) {
	Vector3 pos = this->model.getTranslation(); //this->position_world()
	Vector3 characterTargetCenter = pos + this->targetMove + Vector3(0.0f, 1.0f, 0.0f);
	Vector3 coll;
	Vector3 collnorm;
	if (entity->mesh->testSphereCollision(entity->model, characterTargetCenter, 0.5, coll, collnorm)) {
		//coll = Vector3(-coll.x,coll.y,coll.z);
		Vector3 push_away = normalize(coll-characterTargetCenter)*Game::instance->elapsed_time;
		this->targetPos = pos - push_away;
		this->targetPos.y = pos.y;
		this->targetMove = Vector3((-push_away.x), 0, (-push_away.z));
		//this->targetMove = Vector3(-(this->targetMove.x), 0, -this->targetMove.z);
		//velocity = reflect(velocity, collnorm)*0.95;
		return TRUE;
	}

	else	
		return FALSE;
}

void Player::boundingSelected()
{
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;
	drawText(screen_x, screen_y, ".", Vector3(1, 1, 1), 3);
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

void Player::shoot()
{
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
	if (entity) {
		//entity->visible = FALSE;
		Mesh* mesh = new Mesh();
		mesh->createPlane(0.05f);
		Texture* texture = Texture::Get("data/bullet.png");
		Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		EntityMesh* entity_sh = new EntityMesh();
		entity_sh->mesh = mesh;
		entity_sh->texture = texture;
		entity_sh->shader = shader;
		entity_sh->render_always = TRUE;
		entity_sh->visible = TRUE;

		//Vector3 push_away = normalize(coll - characterTargetCenter);
		entity_sh->model.setTranslation(entity_col.x, entity_col.y, entity_col.z);
		entity_sh->model.setFrontAndOrthonormalize(entity_normal); //error...
		entity_sh->model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
		Scene::instance->addBulletHole(entity_sh);
	}
}
