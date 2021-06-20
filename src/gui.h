#pragma once
#include "utils.h"
#include "framework.h"
#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "animation.h"
#include "shader.h"

class Gui : public Entity
{
public:
	
	Texture* texture;
	Shader* shader;
	Vector4 color;
	Camera* cam2D;
	Mesh quad;
	bool invert;
	bool hover;

	Gui() {};
	Gui(const char* texture, bool invert);
	virtual void render(float x, float y, float w, float h, bool can_hover = false);
	//void update(float dt);

};

