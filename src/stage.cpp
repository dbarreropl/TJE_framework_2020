#include "stage.h"
#include "input.h"

Stage* Stage::current_stage = NULL;

Stage::Stage()
{
}

//IntroStage
void IntroStage::render(std::vector<Stage*> stages) {

	drawText(100, 450, "LOADING...", Vector3(1, 1, 1), 8);

}

void IntroStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	
	Scene::instance->loadScene();
	this->current_stage = stages[2];
	
}

//TutoStage
void TutoStage::render(std::vector<Stage*> stages) {


}

void TutoStage::update(double seconds_elapsed, std::vector<Stage*> stages) {


}

//MenuStage
void MenuStage::render(std::vector<Stage*> stages) {
	
}

void MenuStage::update(double seconds_elapsed, std::vector<Stage*> stages) {


	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //if key ESC was pressed
	{
		
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) //if key up
	{

	}

	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{

	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) //if key space
	{
		
	}
}

//PlayStage
void PlayStage::render(std::vector<Stage*> stages) {

	Player* player = (Player*)Scene::instance->players[0];
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	if(Scene::instance->mode == 1)
		player->updateCamera();

	//render
	Scene::instance->entities[0]->render(); //sky

	for (int i = 1; i < Scene::instance->entities.size(); i++) { //entities
		Scene::instance->entities[i]->render();
	}
	
	for (int i = 0; i < Scene::instance->bullet_holes.size(); i++) { //bullet_holes
		Scene::instance->bullet_holes[i]->render();
	}

	Scene::instance->players[0]->render(); //player

	for (int i = 0; i < Scene::instance->characters.size(); i++) { //enemies
		Scene::instance->characters[i]->render();
	}

	//player shoot direction
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;
	drawText(screen_x, screen_y-20, ".", Vector3(1, 1, 1), 4);

	//object selected camara
	//player->boundingSelected();

	//Draw the floor grid
	if (Scene::instance->mode == 0)
		drawGrid(); 
	
	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

	Camera* camera = (Camera*)Scene::instance->cameras[0];
	Player* player = (Player*)Scene::instance->players[0];

	//set yaw and pitch player	
	player->yaw = (Input::mouse_delta.x * seconds_elapsed * 0.75f);
	player->pitch = clamp(player->pitch + Input::mouse_delta.y * seconds_elapsed * 0.75f, -88 * DEG2RAD, 88 * DEG2RAD);

	//move player
	player->isColiding = FALSE;
	Matrix44 playerRot;
	playerRot.setRotation(player->yaw*DEG2RAD, Vector3(0,1,0));
	Vector3 playerFront = playerRot.rotateVector(Vector3(0.0f,0.0f,-1.0f));
	Vector3 playerRight = playerRot.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
	Vector3 playerSpeed = Vector3(0,0,0);


	Vector3 speed = player->velocity * seconds_elapsed;
	
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 1.75; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		playerSpeed = playerSpeed - (playerFront* speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) { 
		playerSpeed = playerSpeed + (playerFront * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) { 
		playerSpeed = playerSpeed + (playerRight * speed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) { 
		playerSpeed = playerSpeed - (playerRight * speed);
	}

	if (playerSpeed.x != 0 || playerSpeed.y != 0 || playerSpeed.z != 0) {
		player->isMoving = TRUE;
		player->time_walk = Game::instance->time;
	}
	else {
		player->isMoving = FALSE;
	}

	player->targetMove = playerSpeed;

	player->aux = false;
	player->height_floor = -0.05;
	
	//player colision
	for (int i = 1; i < Scene::instance->entities.size(); i++) {
		EntityMesh* entity = (EntityMesh*)Scene::instance->entities[i];
		if (entity->visible == TRUE)
			if (player->testCollision(entity, player->targetMove))
				player->onCollision(entity);
	}
	
	//lanzar rayo de pies a abajo, si distancia es mas grande que 0.1 bajar -0.05, si es mas pequeño hacer push away
	Vector3 pos = player->model.getTranslation(); //this->position_world()
	Vector3 characterTargetCenter = pos + player->targetMove + Vector3(0.0f, 0.2f, 0.0f);
	Vector3 dir = Vector3(0.0f, -5.0f, 0.0f);
	float near_y = 10;
	for (int i = 1; i < Scene::instance->entities.size(); i++) {
		EntityMesh* entity = (EntityMesh*)Scene::instance->entities[i];
		Vector3 col;
		Vector3 normal;
		if (entity->visible == TRUE)
			if (entity->mesh->testRayCollision(entity->model, pos, dir, col, normal, 10)) {
				float dist_obj = pos.y-(col.y + normal.y);
				if (dist_obj < near_y)
					near_y = dist_obj;
			}
	}
	
	//near_y = clamp(near_y,0,1);
	if (near_y > player->height_floor && pos.y>0.1)
			player->targetMove.y = -0.05;

	//std::cout << near_y << std::endl;

	player->model.rotate((player->yaw), Vector3(0.0f, -1.0f, 0.0f));
	player->movePlayer(player->targetMove);


	//shoot
	if (Input::mouse_state & SDL_BUTTON_RIGHT) //is left button pressed?
	{
		player->shoot();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Y))
	{
		player->aux = true;
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		
	}

	Input::centerMouse();
	SDL_ShowCursor(false);
}

//EndStage
void EndStage::render(std::vector<Stage*> stages) {

}

void EndStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		
	}
}