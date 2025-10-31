#pragma once

#include "Globals.h"
#include "Module.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> parent of a0ed02b (iu)
#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
<<<<<<< HEAD
=======
#include "ModulePhysics.h"
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
class PhysicEntity;

>>>>>>> parent of a0ed02b (iu)

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();

<<<<<<< HEAD
<<<<<<< HEAD
    void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;
=======
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;
>>>>>>> parent of a0ed02b (iu)

public:

<<<<<<< HEAD
    PhysBody* death_sensor;
=======
    void CreateWalls();

private:
    // Texturas
    Texture tex_ball;
    Texture tex_boardL;
    Texture tex_boardR2;
    Texture tex_game_back2;
    Texture tex_crate;

    // Cuerpos físicos
    PhysBody* ball;
    PhysBody* left_flipper;
    PhysBody* right_flipper;
    PhysBody* ground_anchor;

    // Articulaciones
    b2RevoluteJoint* left_joint;
    b2RevoluteJoint* right_joint;
    void DrawCollisionShapes();
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
	std::vector<PhysicEntity*> entities;

	PhysBody* sensor;
	bool sensed;

	Texture2D circle;
	Texture2D box;
	Texture2D rick;
	Texture2D paleta;
	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
>>>>>>> parent of a0ed02b (iu)
};