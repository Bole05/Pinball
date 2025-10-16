#pragma once
#include "Module.h"
#include "Globals.h"
#include "raylib.h"

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
    float left_flipper_rotation = 0.0f;
    float right_flipper_rotation = 0.0f;
};