#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

public:

	std::vector<PhysicEntity*> entities;

	PhysBody* sensor;
	bool sensed;

	PhysBody* background;
	PhysBody* ball;
	// Añade variables para los flippers
	PhysBody* left_flipper;
	PhysBody* right_flipper;
	PhysBody* left_flipper_anchor;
	PhysBody* right_flipper_anchor;

	Texture2D tex_ball;
	Texture2D circle;
	Texture2D box;
	Texture2D rick;
	Texture2D paleta;
	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
};