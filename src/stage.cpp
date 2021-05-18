#include "stage.h"
#include "input.h"

Stage* Stage::current_stage = NULL;

Stage::Stage()
{
}

//IntroStage
void IntroStage::render(std::vector<Stage*> stages) {

	drawText(100, 100, "LOADING", Vector3(1, 1, 1), 15);

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
	
	for (int i = 2; i < Scene::instance->entities.size(); i++) {
		Scene::instance->entities[i]->render();
	}
	//Scene::instance->entities[2]->render();
	
	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, std::vector<Stage*> stages) {

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

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
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