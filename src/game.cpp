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

	//sky
	Entity* sky = new EntityMesh("data/skydome.obj", "data/skydome.png");
	sky->model.scale(24.973, 24.973, 24.973);
	sky->render_always = 1;
	sky->setType(0);
	Scene::instance->addEntity(sky);

	//target
	Entity* target = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target->model.setTranslation(-17.4,1.6,-7.4);
	Scene::instance->addTarget(target);

	Entity* target2 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target2->model.setTranslation(15.1, 4.1, -12.9);
	target2->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target2);

	Entity* target3 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target3->model.setTranslation(15.6, 3.4, -35.5);
	target3->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target3);

	Entity* target4 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target4->model.setTranslation(-8.1, 2.7, -30);
	Scene::instance->addTarget(target4);

	Entity* target5 = new EntityMesh("data/biglib/target/Target.obj", "data/biglib/target/Target.png");
	target5->model.setTranslation(7.4, 4.5, 10.7);
	target5->model.rotate(DEG2RAD * 90.0f, Vector3(0, 1, 0));
	Scene::instance->addTarget(target5);

	for (int i = 0; i < Scene::instance->targets.size(); i++) { //targets
		Scene::instance->targets[i]->name= "Target";
		Scene::instance->targets[i]->visible = FALSE;
	}

	//player
	Entity* player = new Player("data/biglib/character.mesh", "data/biglib/WesternPack_renamed/texture.tga");
	Vector3 initialPos = Vector3(4.700, 0, 2.400);
	player->model.setTranslation(initialPos.x, initialPos.y, initialPos.z);
	player->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	Scene::instance->addEntity(player);
	//cam to player pos
	Player* player_s = (Player*)Scene::instance->players[0];
	player_s->initialPos = initialPos;

	//characters
	Entity* BusinessMan = new Character(2,"data/biglib/Characters/Business_Man.mesh", "data/gui/BusinessMan.png", "data/gui/BusinessMan_done.png", "data/biglib/old_man_idle.skanim");
	BusinessMan->model.setTranslation(4.700, 0, 4.400);
	BusinessMan->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	BusinessMan->name = "BusinessMan";
	Scene::instance->addEntity(BusinessMan);
	Character* BusinessMan_character = (Character*)Scene::instance->characters.back();
	BusinessMan_character->setMissionGui("data/gui/BusinessMan_mission_in.png", "data/gui/BusinessMan_mission_next.png", "data/gui/BusinessMan_mission_done.png");

	Entity* CowBoy = new Character(3,"data/biglib/Characters/CowBoy.mesh", "data/gui/text.png", "data/gui/text.png", "data/biglib/arm_stretching.skanim");
	CowBoy->model.setTranslation(12.800, 0, -12.4);
	CowBoy->model.rotate(DEG2RAD * 90.f, Vector3(0.0f, 1.0f, 0.0f));
	CowBoy->name = "CowBoy";
	Scene::instance->addEntity(CowBoy);

	Entity* CowGirl = new Character(4,"data/biglib/Characters/CowGirl.mesh", "data/gui/text.png", "data/gui/text.png", "data/biglib/briefcase_idle.skanim");
	CowGirl->model.setTranslation(-2.2, 0, -13.9);
	CowGirl->name = "CowGirl";
	Scene::instance->addEntity(CowGirl);

	Entity* GunMan = new Character(5,"data/biglib/Characters/GunMan.mesh", "data/gui/text.png", "data/gui/text.png", "data/biglib/sitting.skanim");
	GunMan->model.setTranslation(8.7, 0.2, -19.4);
	GunMan->name = "GunMan";
	Scene::instance->addEntity(GunMan);

	Entity* Sheriff = new Character(1,"data/biglib/Characters/Sheriff.mesh", "data/gui/Sheriff.png","data/gui/Sheriff_done.png", "data/biglib/drunk_idle.skanim");
	Sheriff->model.setTranslation(3.8, 0, -37.9);
	Sheriff->model.rotate(DEG2RAD * 90.f, Vector3(0.0f, 1.0f, 0.0f));
	Sheriff->name = "Sheriff";
	Scene::instance->addEntity(Sheriff);
	Character* Sheriff_character = (Character*)Scene::instance->characters.back();
	Sheriff_character->setMissionGui("data/gui/Sheriff_mission_in.png", "data/gui/Sheriff_mission_next.png","data/gui/Sheriff_mission_done.png");

	Entity* Woman = new Character(6,"data/biglib/Characters/Woman.mesh", "data/gui/text.png", "data/gui/text.png", "data/biglib/breathing_idle.skanim");
	Woman->model.setTranslation(-9.700, 0, -38.49);
	Woman->model.rotate(DEG2RAD * 180.f, Vector3(0.0f, 1.0f, 0.0f));
	Woman->name = "Woman";
	Scene::instance->addEntity(Woman);

	Entity* WorkingGirl = new Character(7,"data/biglib/Characters/WorkingGirl.mesh", "data/gui/text.png", "data/gui/text.png", "data/biglib/jazz_dancing.skanim");
	WorkingGirl->model.setTranslation(-17.2, 0.5, -13.4);
	WorkingGirl->model.rotate(DEG2RAD * 270.f, Vector3(0.0f, 1.0f, 0.0f));
	WorkingGirl->name = "WorkingGirl";
	Scene::instance->addEntity(WorkingGirl);	
	
	//Gui
	Entity* title = new Gui("data/gui/title.png",false);
	Scene::instance->addEntity(title);

	Entity* title2 = new Gui("data/gui/main_play.png", false);
	Scene::instance->addEntity(title2);

	Entity* paper = new Gui("data/gui/paper.png",true);
	Scene::instance->addEntity(paper);

	Entity* resume = new Gui("data/gui/resume.png", false);
	Scene::instance->addEntity(resume);

	Entity* exit = new Gui("data/gui/exit.png", false);
	Scene::instance->addEntity(exit);

	Entity* loading = new Gui("data/gui/loading2.png", false); //5
	Scene::instance->addEntity(loading);

	Entity* text = new Gui("data/gui/text.png", false);
	Scene::instance->addEntity(text);

	Entity* talk_key = new Gui("data/gui/talk.png", false);
	Scene::instance->addEntity(talk_key);

	Entity* title3 = new Gui("data/gui/main_exit.png", false);
	Scene::instance->addEntity(title3);

	Entity* pick_up = new Gui("data/gui/pick_up.png", false);
	Scene::instance->addEntity(pick_up);

	Entity* scope = new Gui("data/gui/scope.png", false); //10
	Scene::instance->addEntity(scope);

	Entity* scope_shoot = new Gui("data/gui/scope_shoot.png", false);
	Scene::instance->addEntity(scope_shoot);

	Entity* bullet_0 = new Gui("data/gui/bullet_0.png", false);
	Scene::instance->addEntity(bullet_0);

	Entity* bullet_1 = new Gui("data/gui/bullet_1.png", false);
	Scene::instance->addEntity(bullet_1);

	Entity* bullet_2 = new Gui("data/gui/bullet_2.png", false);
	Scene::instance->addEntity(bullet_2);

	Entity* bullet_3 = new Gui("data/gui/bullet_3.png", false); //15
	Scene::instance->addEntity(bullet_3);

	Entity* bullet_4 = new Gui("data/gui/bullet_4.png", false);
	Scene::instance->addEntity(bullet_4);

	Entity* bullet_5 = new Gui("data/gui/bullet_5.png", false);
	Scene::instance->addEntity(bullet_5);

	Entity* bullet_6 = new Gui("data/gui/bullet_6.png", false);
	Scene::instance->addEntity(bullet_6);

	Entity* card = new Gui("data/gui/card.png", false);
	Scene::instance->addEntity(card);

	Entity* object_1 = new Gui("data/gui/object_1.png", false); //20
	Scene::instance->addEntity(object_1);

	Entity* object_2 = new Gui("data/gui/object_2.png", false); 
	Scene::instance->addEntity(object_2);

	Entity* object_3 = new Gui("data/gui/object_3.png", false);
	Scene::instance->addEntity(object_3);

	Entity* object_4 = new Gui("data/gui/object_4.png", false);
	Scene::instance->addEntity(object_4);

	Entity* object_5 = new Gui("data/gui/object_5.png", false);
	Scene::instance->addEntity(object_5);

	Entity* object_6 = new Gui("data/gui/object_6.png", false); //25
	Scene::instance->addEntity(object_6);

	Entity* object_7 = new Gui("data/gui/object_7.png", false);
	Scene::instance->addEntity(object_7);

	Entity* object_8 = new Gui("data/gui/object_8.png", false);
	Scene::instance->addEntity(object_8);

	Entity* object_9 = new Gui("data/gui/object_9.png", false);
	Scene::instance->addEntity(object_9);

	Entity* object_9p = new Gui("data/gui/object_9+.png", false);
	Scene::instance->addEntity(object_9p);

	Entity* gameover = new Gui("data/gui/gameover.png", false); //30
	Scene::instance->addEntity(gameover);

	Entity* won = new Gui("data/gui/won.png", false);
	Scene::instance->addEntity(won);

	Entity* close = new Gui("data/gui/close.png", false);
	Scene::instance->addEntity(close);

	Entity* tutorial = new Gui("data/gui/tutorial.png", false);
	Scene::instance->addEntity(tutorial);

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
	if (Input::wasKeyPressed(SDL_SCANCODE_L) && Stage::current_stage->getStage()==2)
	{
		if (Scene::instance->mode == 0) {
			Scene::instance->mode = 1;
			player->updateCamera(); //camara set to player pos
		}
		else
			Scene::instance->mode = 0;
	}

	//free camera
	if (Scene::instance->mode == 0) {
		//mouse input to rotate the cam
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		//async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
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
