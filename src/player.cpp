#include "player.h"
#include "scene.h"


void Player::movePlayer(Vector3 move) {
	
	this->moving = true;
	this->model.translate(move.x,move.y,move.z);

}


