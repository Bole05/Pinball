#pragma once
#include "Module.h"
#include "ModulePhysics.h"

class ModuleGame : public Module
{
public:
    ModuleGame(Application* app, bool start_enabled = true);
    ~ModuleGame();

    bool Start();
    update_status Update();
    bool CleanUp();

    void CreateWalls();

private:
    // Texturas
    Texture tex_ball;
    Texture tex_boardL;
    Texture tex_boardR;
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
};