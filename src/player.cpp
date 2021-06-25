#include "player.h"
#include "scene.h"
#include "game.h"
#include "input.h"
#include "audio.h"

void Player::render()
{
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float time = Game::instance->time;

	if (shader && this->visible == TRUE)
	{
		Skeleton result = shot->skeleton;

		if (isReloading) {
			reload->assignTime(time - time_reload);
			blendSkeleton(&result, &reload->skeleton, 1.0f, &result, LEFT_ARM);
			if ((time - time_reload) > reload->duration) {
				isReloading = false;
				bullets = 6;
				reload->assignTime(0);
			}
		}
		else {
			reload->assignTime(0);
			blendSkeleton(&result, &reload->skeleton, 1.0f, &result, LEFT_ARM);
		}

		shot->assignTime(0);
		walk->assignTime(time_walk);
		if (isShooting) {
			shot->assignTime(time - time_shot);
			if ((time - time_shot) > shot->duration) {
				isShooting = false;
				shot->assignTime(0);
			}
		}
		if (isMoving) {
			walk->assignTime(time_walk);
		}
		walk->assignTime(0);
		blendSkeleton(&result, &walk->skeleton, 0.5f, &result);
		//walk->assignTime(0, RIGHT_ARM);

		//gun
		//Vector3 center = (camera->center - camera->eye).normalize();
		//Vector3 pos = camera->eye + center;
		Matrix44& RightHand = result.getBoneMatrix("mixamorig_RightHand", false);
		RightHand.rotate(0.2, Vector3(0.3, -0.35, 1.12));
		RightHand.rotate(pitch*0.8, Vector3(0.3, -0.35, 1.12));

		RightHand.translate(0,-pitch*0.04,0);		

		if (pitch > 0.1) {
			RightHand.translate((pitch - 0.1) * -0.2, (pitch-0.1) * 0.4, 0);
		}

		//Matrix44& RightForeArm = result.getBoneMatrix("mixamorig_RightArm", false);
		//RightForeArm.translate(0, -pitch * 0.5, 0);

		gun.model = model;
		gun.model.translate(-0.03, 0.01, -0.2);
		gun.model = RightHand * gun.model;
		gun.model.rotate(70 * DEG2RAD, Vector3(0, 0, 1));
		gun.model.rotate(55 * DEG2RAD, Vector3(0, 1, 0));
		//gun.model.rotate(time * 100 * DEG2RAD, Vector3(1, 0, 0));
		//gun.model.setTranslation(pos.x, pos.y, pos.z);
		gun.render();

		//shader = Shader::current;
		shader->disable();
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", this->model);
		shader->setUniform("u_time", time_walk);
		//do the draw call

		Matrix44& Head = result.getBoneMatrix("mixamorig_Head");
		Head.scale(0,0,0);
		Matrix44& LeftArm = result.getBoneMatrix("mixamorig_LeftArm");

		LeftArm.translate(-0.1,0.04,0.25);
		LeftArm.rotate(-0.85,Vector3(1,0,0));
		LeftArm.rotate(0.65, Vector3(0, 1, 0));
		//LeftArm.rotate(z, Vector3(1, 0, 1));
	
		mesh->renderAnimated(GL_TRIANGLES, &result);
		//shader->disable();
	}
}

void Player::movePlayer(Vector3 move) {

	if (isColiding == TRUE)
		this->model.translateGlobal(move.x, move.y, move.z);
	else
		this->model.translate(move.x, move.y, move.z);

}

void Player::updateCamera()
{
	Camera* cam = (Camera*)Scene::instance->cameras[0];
	Vector3 player_pos = this->model.getTranslation();
	
	Matrix44 pitch;
	//this->model.rotate(this->pitch * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
	pitch.rotate(this->pitch , Vector3(1.0f,0.0f,0.0f));
	Vector3 forward = pitch.rotateVector(Vector3(0.0f, 0.0f, 1.0f));
	forward = this->model.rotateVector(forward);
	//forward.x = forward.x - 0.1;

	Vector3 eye = this->model * Vector3(-0.1f, 1.4f, 0.15f);
	Vector3 center = eye + forward;

	Vector3 up = Vector3(0.0f,1.0f,0.0f);
	cam->lookAt(eye,center,up);
	//cam->rotate(this->yaw, Vector3(0.0f, -1.0f, 0.0f));
	//cam->rotate(this->pitch, cam->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
}

void Player::onCollision(Entity* entity)
{
	if(entity->type==0)
		isColiding = TRUE;
}

bool Player::testCollision(EntityMesh* entity, Vector3 targetMove) {
	Vector3 pos = this->model.getTranslation(); //this->position_world()
	Vector3 characterTargetCenter = pos + targetMove + Vector3(0.0f, 0.2f, 0.0f);
	Vector3 coll;
	Vector3 collnorm;

	if (entity->mesh->testSphereCollision(entity->model, characterTargetCenter, 0.4, coll, collnorm)) {
		Vector3 push_away = normalize(coll - characterTargetCenter) *Game::instance->elapsed_time;
		this->targetMove = Vector3((-push_away.x), -(push_away.y*5), (-push_away.z));
		velocity = reflect(velocity, collnorm)*0.95;
		//this->height_floor = characterTargetCenter.y;
		aux = true;
		this->height_floor = (push_away.y*5);
		return TRUE;
	}
	else {
		velocity = Vector3(5.0f,5.0f,5.0f);
		return FALSE;
	}
}

void Player::pickUp()
{
	Player* player = (Player*)Scene::instance->players[0];
	player->nearObject = false;
	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(screen_x, screen_y, Game::instance->window_width, Game::instance->window_height);
	for (int i = 0; i < Scene::instance->entities.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->entities[i];
		if (current->can_pickUp) {
			Vector3 col;
			Vector3 normal;
			if (current->mesh->testRayCollision(current->model, origin, dir, col, normal, 2)) {
				//current->mesh->renderBounding(current->model);
				if (current->visible == TRUE) {
					player->nearObject = true;
					if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
						Audio::Play("data/audio/pick_up.wav", 2000, false);
						current->visible = FALSE;
						addObject(current);
						break;
					}
				}
			}
		}
	}
}

void Player::addObject(Entity* entity)
{
	if (entity->name=="SM_Prop_Card") {
		Gui* card = (Gui*)Scene::instance->guis[19];
		if (hasObject(entity->name)==0) { //if doesnt has object add it
			card->name = entity->name;
			objects.push_back(card);
		}
		else //+1
		{
			if(card->number<10)
				card->number += 1;
		}
	}
}

bool Player::hasObject(std::string object) {

	for (int i = 0; i < objects.size(); i++) {
		Gui* player_object = (Gui*)objects[i];
		if (object == player_object->name)
			return true;
	}
	return false;
}

float Player::numberObjects(std::string object) {

	for (int i = 0; i < objects.size(); i++) {
		Gui* player_object = (Gui*)objects[i];
		if (object == player_object->name)
			return player_object->number;
	}
}

void Player::shoot()
{
	time_shot = Game::instance->time;
	isShooting = true;
	if(bullets>0)
		bullets -= 1;

	//entities colision
	std::vector <Entity*> entities;
	std::vector <Vector3> collisions;
	std::vector <Vector3> normals;
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
			entities.push_back(current);
			collisions.push_back(col);
			normals.push_back(normal);
		}
	}

	//character colision
	for (int i = 0; i < Scene::instance->characters.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->characters[i];
		Vector3 col;
		Vector3 normal;
		if (current->mesh->testRayCollision(current->model, origin, dir, col, normal, 10)) {
			entities.push_back(current);
			collisions.push_back(col);
			normals.push_back(normal);
		}
	}

	//target colision
	for (int i = 0; i < Scene::instance->targets.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->targets[i];
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

	if (entity && entity->type == 4) {
		Character* character = (Character*)entity;
		character->dead = TRUE;
		character->time_dead = Game::instance->time;
		if (entity->name == "BusinessMan")
			won = true;
		else
			game_over = true;
	}
	else if (entity && entity->name=="Target") {
		entity->visible = FALSE;
	}
	else {
		//create bullet hole
		if (entity) {
			Mesh* mesh = Mesh::Get("data/bullet.obj");
			Texture* texture = Texture::Get("data/bullet2.png");
			Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			EntityMesh* entity_sh = new EntityMesh();
			entity_sh->mesh = mesh;
			entity_sh->texture = texture;
			entity_sh->shader = shader;
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
}
