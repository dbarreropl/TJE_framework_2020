#pragma once
#include "utils.h"
#include "framework.h"
#include "game.h"
#include "scene.h"

class Stage
{
public:

	static Stage* current_stage;

	enum STAGE {
		IntroStage,
		MenuStage,
		PlayStage,
		TutoStage,
		EndStage
	};

	STAGE actual_stage;

	virtual void render(std::vector<Stage*> stages) {}; //empty body
	virtual void update(double seconds_elapsed, std::vector<Stage*> stages) {}; //empty body
	virtual int getStage() { return (int)actual_stage; };
	Stage();
};


class IntroStage : public Stage {
public:
	Image image_intro;

	STAGE actual_stage = (STAGE)0;
	IntroStage() {};
	void render(std::vector<Stage*> stages);
	void update(double seconds_elapsed, std::vector<Stage*> stages);
	int getStage() { return (int)actual_stage; };
};

class TutoStage : public Stage {
public:
	Image image_tuto;

	STAGE actual_stage = (STAGE)3;
	TutoStage() {};
	void render(std::vector<Stage*> stages);
	void update(double seconds_elapsed, std::vector<Stage*> stages);
	int getStage() { return (int)actual_stage; };
};

class MenuStage : public Stage {
public:

	STAGE actual_stage = (STAGE)1;

	enum MENU {
		RESUME,
		EXIT
	};
	MENU menu = RESUME;

	void render(std::vector<Stage*> stages);
	void update(double seconds_elapsed, std::vector<Stage*> stages);
	int getStage() { return (int)actual_stage; };
};

class PlayStage : public Stage {
public:

	STAGE actual_stage = (STAGE)2;
	int level=1;

	void render(std::vector<Stage*> stages);
	void update(double seconds_elapsed, std::vector<Stage*> stages);
	int getStage() { return (int)actual_stage; };
};

class EndStage : public Stage {
public:
	Image image_end;
	Image image_gameover;

	STAGE actual_stage = (STAGE)4;
	EndStage() {};
	void render(std::vector<Stage*> stages);
	void update(double seconds_elapsed, std::vector<Stage*> stages);
	int getStage() { return (int)actual_stage; };
};