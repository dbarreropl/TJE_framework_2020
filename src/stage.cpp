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

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	Gui* title = (Gui*)Scene::instance->guis[0];
	title->render(width / 2, height / 2.8, width * 0.7, height * 0.7);
	Gui* main_play = (Gui*)Scene::instance->guis[1];
	main_play->render(width *0.65, height *0.8, width * 0.18, height * 0.14, true);
	Gui* main_exit = (Gui*)Scene::instance->guis[8];
	main_exit->render(width * 0.35, height * 0.8, width * 0.18, height * 0.14, true);
}

void MainStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	if (!music_playing) {
		Audio::Play("data/audio/music.wav", 2000, true);
		music_playing = true;
	}

	Player* player = (Player*)Scene::instance->players[0];
	player->model.setTranslation(player->initialPos.x, player->initialPos.y, player->initialPos.z);
	player->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));

	Camera* camera = (Camera*)Scene::instance->cameras[0];
	if(camera->eye.z > -40)
		camera_move-=0.002;

	Vector3 eye = Vector3(0.0f, 4.0f, camera_move);
	Vector3 center = eye + Vector3(0,-0.5,-1);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	camera->lookAt(eye, center, up);

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //if key space
	{
		Game::instance->must_exit = true;
	}

	Gui* main_play = (Gui*)Scene::instance->guis[1];
	Gui* main_exit = (Gui*)Scene::instance->guis[8];
	if ((Input::mouse_state & SDL_BUTTON_RIGHT)) //Exit
	{
		if(main_exit->hover)
			Game::instance->must_exit = true;
		else if(main_play->hover){
			HCHANNEL music = Audio::GetChannel("data/audio/music.wav");
			Audio::Stop(music);
			camera_move = 0;
			music_playing = false;
			this->current_stage = stages[2];
		}
	}

	SDL_ShowCursor(true);
}

//MenuStage
void MenuStage::render(std::vector<Stage*> stages) {
	stages[2]->render(stages);
	//drawText(100, 450, "MENU", Vector3(1, 1, 1), 4);

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;
	Gui* icon = (Gui*)Scene::instance->guis[2];
	icon->render(width /2, height /2, width*0.45, height*0.6);
	Gui* resume = (Gui*)Scene::instance->guis[3];
	//resume->render(width / 2, height / 2.8, width, height, true);
	resume->render(width / 2, height / 2.7, width/3.5, height/7, true);
	Gui* exit = (Gui*)Scene::instance->guis[4];
	exit->render(width / 2, height / 1.7, width/3.5, height/7, true);

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

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;

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

	player->render(); //player

	for (int i = 0; i < Scene::instance->characters.size(); i++) { //characters
		Scene::instance->characters[i]->render();
	}

	//player shoot direction
	float screen_x = Input::mouse_position.x + Input::mouse_delta.x;
	float screen_y = Input::mouse_position.y + Input::mouse_delta.y;

	if (current_stage->getStage() == 2) {
		Gui* scope;
		if (!player->isShooting)
			scope = (Gui*)Scene::instance->guis[10];
		else
			scope = (Gui*)Scene::instance->guis[11];
		scope->render(screen_x, screen_y, width, height, false);
	}

	//talk to a character
	Character* character;
	player->nearCharacter = false;
	bool isTalking = player->isTalking;
	player->isTalking=false;
	for (int i = 0; i < Scene::instance->characters.size(); i++) {
		float dist = Scene::instance->characters[i]->model.getTranslation().distance(player->position_world());
		if (dist < 1.5) {
			player->nearCharacter = true;
			character = (Character*)Scene::instance->characters[i];
			player->isTalking=isTalking;
			break;
		}
	}

	if (player->nearCharacter) {
		if (!player->isTalking) {
			Gui* talk_key = (Gui*)Scene::instance->guis[7];
			talk_key->render(width / 2, height / 1.2, width * 0.35, height * 0.35);
		}
		else {
			character->text->render(width / 2, height / 1.3, width * 0.6, height * 0.3, false);
			character->active = true;
		}
	}

	if (player->nearObject) {
		Gui* pick_up = (Gui*)Scene::instance->guis[9];
		pick_up->render(width / 2, height / 1.2, width * 0.35, height * 0.35);
	}

	Gui* bullet_1 = (Gui*)Scene::instance->guis[12+player->bullets];
	bullet_1->render(width/2, height/1.9, width, height);

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

	//wait 0.5sec to shoot
	if (player->canShoot == false) {
		if(wait==0)
			wait = Game::instance->time;
		if ((Game::instance->time - wait) > 0.5) {
			player->canShoot = true;
			wait = 0;
		}
	}
	if (player->bullets == 0) //no bullets
		player->canShoot = FALSE;

	//shoot
	if (Input::mouse_state & SDL_BUTTON_RIGHT) //is left button pressed?
	{
		if (!player->isShooting && player->canShoot && !player->isReloading) {
			Audio::Play("data/audio/shot_revolver.wav", 2000, false);
			player->shoot();
		}
	}

	//reload
	if (Input::wasKeyPressed(SDL_SCANCODE_R) && !player->isReloading && player->bullets<6)
	{
		Audio::Play("data/audio/reload_revolver.wav", 2000, false);
		player->isReloading = TRUE;
		player->time_reload = Game::instance->time;
	}

	//go to menu
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		player->canShoot = false;
		this->current_stage = stages[1];
	}
	
	//action
	player->pickUp(); //Gui object near and E to pick up object
	if (Input::wasKeyPressed(SDL_SCANCODE_E))
	{
		if (player->nearCharacter) //talk
			player->isTalking = true;
		//else //pick up object
		//	player->pickUp();
	}

	//game over or won
	if(player->game_over || player->won)
		this->current_stage = stages[4];

	if (Input::wasKeyPressed(SDL_SCANCODE_T))
	{
		player->x += 0.05;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Y))
	{
		player->x -= 0.05;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_U))
	{
		player->y += 0.05;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_I))
	{
		player->y -= 0.05;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_O))
	{
		player->z += 0.05;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_P))
	{
		player->z -= 0.05;
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
	Player* player = (Player*)Scene::instance->players[0];
	if(player->won)
		drawText(100, 450, "You win!", Vector3(1, 1, 1), 4);
	else
		drawText(100, 450, "you lose :(", Vector3(1, 1, 1), 4);
}

void EndStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	Player* player = (Player*)Scene::instance->players[0];
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		player->won = FALSE;
		player->game_over = FALSE;
		this->current_stage = stages[3];	
	}
}