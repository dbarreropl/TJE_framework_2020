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

	Scene::instance->entities[1]->render(); //sky

	for (int i = 2; i < Scene::instance->entities.size(); i++) {
		Scene::instance->entities[i]->render();
	}

	if (Scene::instance->mode == 0)
		drawGrid(); //Draw the floor grid

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2); //render the FPS, Draw Calls, etc

}

void PlayStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

	//change free camera
	if (Input::wasKeyPressed(SDL_SCANCODE_L))
	{
		if (Scene::instance->mode == 0)
			Scene::instance->mode = 1;
		else
			Scene::instance->mode = 0;
	}


	//keys
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{

	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{

	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{

	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{

	}

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