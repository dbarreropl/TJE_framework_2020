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

	Scene::instance->entities[0]->render(); //sky
	Scene::instance->players[0]->render();
	for (int i = 1; i < Scene::instance->entities.size(); i++) {
		Scene::instance->entities[i]->render();
	}

	if (Scene::instance->mode == 0)
		drawGrid(); //Draw the floor grid

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2); //render the FPS, Draw Calls, etc

}

void PlayStage::update(double seconds_elapsed, std::vector<Stage*> stages) {
	//update camara respecto player
	Camera* camera = (Camera*)Scene::instance->cameras[0];
	Player* player = (Player*)Scene::instance->players[0];
	Vector3 player_pos = player->model.getTranslation();
	camera->lookAt(Vector3(player_pos.x, player_pos.y + 1.675, player_pos.z - 0.2), Vector3(player_pos.x + 0.3, player_pos.y + 1.675, player_pos.z-2.5), Vector3(0.f, 1.f, 0.f));

	//...player.y como altura rayo colision debajo player

	//colision camera
	/*

	Vector3 origin = cam->eye;
	Vector3 dir = cam->getRayDirection(cam->center.x, cam->center.x, Game::instance->window_width, Game::instance->window_height);
	for (int i = 2; i < Scene::instance->entities.size(); i++) {
		EntityMesh* current = (EntityMesh*)Scene::instance->entities[i];
		Vector3 col;
		Vector3 normal;
		if (current->mesh->testRayCollision(current->model, origin, dir, col, normal, 0.1))
			float a = 1;
	}*/


	//move player
	float speed = seconds_elapsed * 10.0f;
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player->movePlayer(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player->movePlayer(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) player->movePlayer(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player->movePlayer(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //if key ESC was pressed
	{
		
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		
	}

}

//EndStage
void EndStage::render(std::vector<Stage*> stages) {

}

void EndStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		
	}
}