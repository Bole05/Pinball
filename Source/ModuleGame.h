#pragma once
#include "Module.h"
#include "p2Point.h"
#include "vector"

class PhysBody;

class ModuleGame : public Module
{
public:
    ModuleGame(Application* app, bool start_enabled = true);
    ~ModuleGame();

    bool Start();
    update_status Update();
    bool CleanUp();

    void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

private:
    Texture2D background_texture;
    Texture2D ball_texture;
    uint bonus_fx;

    PhysBody* death_sensor;
};