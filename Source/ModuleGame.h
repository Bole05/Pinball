#pragma once
#include "Module.h"
#include "Globals.h"
#include "raylib.h"
#include "ModulePhysics.h"

// Necesario para b2RevoluteJoint
#include "box2d/b2_revolute_joint.h"

class ModuleGame : public Module
{
public:
    ModuleGame(Application* app, bool start_enabled = true);
    ~ModuleGame();

    bool Start();
    update_status Update();
    bool CleanUp();

public:
    // Textures
    Texture2D tex_ball;
    Texture2D tex_boardL;
    Texture2D tex_boardR;
    Texture2D tex_crate;
    Texture2D tex_game_back2;
    Texture2D tex_rick_head;
    Texture2D tex_wheel;

    // Flipper properties
    // float left_flipper_rotation = 0.0f; // Ya no se usa
    // float right_flipper_rotation = 0.0f; // Ya no se usa

    // PhysBodies
    PhysBody* ball;
    PhysBody* left_flipper;
    PhysBody* right_flipper;
    PhysBody* ground_anchor; // Cuerpo estático para anclar los flippers

    // Joints
    b2RevoluteJoint* left_joint;
    b2RevoluteJoint* right_joint;
};