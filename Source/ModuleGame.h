#pragma once
#include "Module.h"
<<<<<<< HEAD
#include "p2Point.h"
#include "vector"

class PhysBody;
=======
#include "ModulePhysics.h"
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90

class ModuleGame : public Module
{
public:
    ModuleGame(Application* app, bool start_enabled = true);
    ~ModuleGame();

    bool Start();
    update_status Update();
    bool CleanUp();

<<<<<<< HEAD
    void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

private:
    Texture2D background_texture;
    Texture2D ball_texture;
    uint bonus_fx;

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
};