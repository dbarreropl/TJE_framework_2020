#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "stage.h"
#include "scene.h"

#include <cmath>

//some globals
Shader* shader = NULL;
float angle = 0;
float mouse_speed = 10.0f;
FBO* fbo = NULL;

//stages
std::vector<Stage*> stages;

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
	camera->setType(1);

	//Create Stages
	stages.push_back(new IntroStage());
	stages.push_back(new MenuStage());
	stages.push_back(new PlayStage());
	stages.push_back(new MainStage());
	stages.push_back(new EndStage());
	stages.push_back(new TutoStage());
	Stage::current_stage = stages[0]; //Intro Stage at start

	//Scene
	Scene::instance = NULL;
	new Scene();

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Scene::instance->addEntity(camera);

	//player
	Entity* player = new Player("data/biglib/character.mesh", "data/biglib/WesternPack_renamed/texture.tga");
	Vector3 initialPos = Vector3(4.700, 0.1, 2.400);
	player->model.setTranslation(initialPos.x, initialPos.y, initialPos.z);
	player->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	Scene::instance->addEntity(player);
	Player* player_s = (Player*)Scene::instance->players[0];
	player_s->initialPos = initialPos; //cam to player pos

	//Inicializamos BASS al arrancar el juego (id_del_device, muestras por segundo, ...)
	if(!BASS_Init(-1, 44100, 0, 0, NULL))
		assert("Error opening sound card");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	shader->enable();

	Stage::current_stage->render(stages);

	shader->disable();

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{

	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//change edit/play mode
	Player* player = (Player*)Scene::instance->players[0];
	/*
	if (Input::wasKeyPressed(SDL_SCANCODE_L) && Stage::current_stage->getStage()==2)
	{
		if (Scene::instance->mode == 0) {
			Scene::instance->mode = 1;
			player->updateCamera(); //camara set to player pos
		}
		else
			Scene::instance->mode = 0;
	}*/

	//free camera
	if (Scene::instance->mode == 0) {
		//mouse input to rotate the cam
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		//async input to move the camera around
		//if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

		//to navigate with the mouse fixed in the middle
		if (mouse_locked)
			Input::centerMouse();

		SDL_ShowCursor(true);
	}
	else
		Stage::current_stage->update(seconds_elapsed, stages);

}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		//case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}
