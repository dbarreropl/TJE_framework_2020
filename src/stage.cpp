#include "stage.h"
#include "input.h"

Stage* Stage::current_stage = NULL;

Stage::Stage()
{
}

//IntroStage
void IntroStage::render(std::vector<Stage*> stages) {
	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	Gui* loading = (Gui*)Scene::instance->guis[5];
	loading->render(width / 2, height / 2, width, height);
	//drawText(100, 450, "LOADING...", Vector3(1, 1, 1), 8);

}

void IntroStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

	Scene::instance->loadScene();
	this->current_stage = stages[3];
	
}

//MainStage
void MainStage::render(std::vector<Stage*> stages) {

	//render
	Scene::instance->entities[0]->render(); //sky

	for (int i = 1; i < Scene::instance->entities.size(); i++) { //entities
		Scene::instance->entities[i]->render();
	}	

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Gui* title = (Gui*)Scene::instance->guis[0];
	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	title->render(width / 2, height / 2.8, width * 0.7, height * 0.7);
	Gui* title2 = (Gui*)Scene::instance->guis[1];
	title2->render(width / 2, height/1.2, width * 0.6, height * 0.6);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

void MainStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	if (!music_playing) {
		Audio::Play("data/audio/music.wav", 2000, true);
		music_playing = true;
	}
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	if(camera->eye.z > -40)
		camera_move-=0.002;

	Vector3 eye = Vector3(0.0f, 4.0f, camera_move);
	Vector3 center = eye + Vector3(0,-0.5,-1);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	camera->lookAt(eye, center, up);

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) //if key space
	{
		HCHANNEL music = Audio::GetChannel("data/audio/music.wav");
		Audio::Stop(music);
		camera_move = 0;
		music_playing = false;
		this->current_stage = stages[2];
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //if key space
	{
		Game::instance->must_exit = true;
	}

	SDL_ShowCursor(true);
}

//MenuStage
void MenuStage::render(std::vector<Stage*> stages) {
	stages[2]->render(stages);
	//drawText(100, 450, "MENU", Vector3(1, 1, 1), 4);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	Gui* icon = (Gui*)Scene::instance->guis[2];
	icon->render(width /2, height /2, width*0.5, height*0.7);
	Gui* resume = (Gui*)Scene::instance->guis[3];
	//resume->render(width / 2, height / 2.8, width, height, true);
	resume->render(width / 2, height / 2.8, width/3.5, height/7, true);
	Gui* exit = (Gui*)Scene::instance->guis[4];
	exit->render(width / 2, height / 1.7, width/3.5, height/7, true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

void MenuStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	Gui* resume = (Gui*)Scene::instance->guis[3];
	Gui* exit = (Gui*)Scene::instance->guis[4];
	Player* player = (Player*)Scene::instance->players[0];

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //Play
	{
		//Input::centerMouse();
		this->current_stage = stages[2];
	}

	if ((Input::mouse_state & SDL_BUTTON_RIGHT) && resume->hover) //Play
	{
		this->current_stage = stages[2];
	}

	if ((Input::mouse_state & SDL_BUTTON_RIGHT) && exit->hover) //Main
	{
		Player* player = (Player*)Scene::instance->players[0];
		player->model.setTranslation(4.700, 0, 4.400);
		player->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));

		HCHANNEL music = Audio::GetChannel("data/audio/ambient.mp3");
		Audio::Stop(music);
		class PlayStage* play = (class PlayStage*)stages[2];
		play->music_playing = false;
		this->current_stage = stages[3];
	}

	SDL_ShowCursor(true);
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

	if (!music_playing) {
		Audio::Play("data/audio/ambient.mp3", 2000, true);
		music_playing = true;
	}

	Camera* camera = (Camera*)Scene::instance->cameras[0];
	Player* player = (Player*)Scene::instance->players[0];

	//set yaw and pitch player
	Vector2 mouse_delta = Vector2(Input::mouse_delta.x, Input::mouse_delta.y);
	if (mouse_delta.x>20 || mouse_delta.y > 20) {
		mouse_delta = Vector2(0,0);
	}
	player->yaw = (mouse_delta.x * seconds_elapsed * 0.75f);
	player->pitch = clamp(player->pitch + mouse_delta.y * seconds_elapsed * 0.75f, -88 * DEG2RAD, 88 * DEG2RAD);

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

	if (player->canShoot == false) {
		if(wait==0)
			wait = Game::instance->time;
		if ((Game::instance->time - wait) > 0.5) {
			player->canShoot = true;
			wait = 0;
		}
	}
	//shoot
	if (Input::mouse_state & SDL_BUTTON_RIGHT) //is left button pressed?
	{
		if (!player->isShooting && player->canShoot) {
			Audio::Play("data/audio/shot_revolver.wav", 2000, false);
			player->shoot();
		}
	}

	//reload
	if (Input::wasKeyPressed(SDL_SCANCODE_R))
	{
		Audio::Play("data/audio/reload_revolver.wav", 2000, false);
	}

	//go to menu
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		player->canShoot = false;
		this->current_stage = stages[1];
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_T))
	{
		player->x += 0.01;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Y))
	{
		player->x -= 0.01;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_U))
	{
		player->y += 0.01;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_I))
	{
		player->y -= 0.01;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_O))
	{
		player->z += 0.01;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_P))
	{
		player->z -= 0.01;
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