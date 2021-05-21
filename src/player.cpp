#include "player.h"
#include "scene.h"
#include "game.h"

void Player::movePlayer(Vector3 move) {
	if (can_move == TRUE) {
		this->model.translate(move.x, move.y, move.z);
		//this->updateCamera();
	}
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

bool Player::testCollision(EntityMesh* entity) {

	Vector3 coll;
	Vector3 collnorm;
	//if(entity)


	
		
	return FALSE;
}