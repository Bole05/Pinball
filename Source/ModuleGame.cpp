#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
    LOG("Loading Game assets");
    bool ret = true;

    // Load textures
    tex_ball = App->renderer->Load("Assets/ball0001.png");
    tex_boardL = App->renderer->Load("Assets/boardL.png");
    tex_boardR = App->renderer->Load("Assets/boardR.png");

    // --- CORRECCIÓN AQUÍ ---
    // Cambiado "game_back2.jpg" a "game_back.jpg"
    tex_game_back2 = App->renderer->Load("Assets/game_back2.jpg");

    // Añadida una comprobación de error
    if (tex_game_back2.id <= 0)
    {
        LOG("ERROR: No se pudo cargar la textura de fondo 'Assets/game_back.jpg'");
    }

    // Initial flipper rotations
    left_flipper_rotation = 20.0f;
    right_flipper_rotation = -20.0f;

    return ret;
}

// Unload assets
bool ModuleGame::CleanUp()
{
    LOG("Unloading Game scene");
    UnloadTexture(tex_ball);
    UnloadTexture(tex_boardL);
    UnloadTexture(tex_boardR);
    UnloadTexture(tex_crate);
    UnloadTexture(tex_game_back2);
    UnloadTexture(tex_rick_head);
    UnloadTexture(tex_wheel);
    return true;
}

// Update game logic and draw
update_status ModuleGame::Update()
{
    // --- Flipper controls ---
    if (IsKeyDown(KEY_LEFT))
    {
        left_flipper_rotation = -35.0f; // Flipper goes up
    }
    else
    {
        left_flipper_rotation = 20.0f; // Flipper goes down
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        right_flipper_rotation = 35.0f; // Flipper goes up
    }
    else
    {
        right_flipper_rotation = -20.0f; // Flipper goes down
    }

    // --- Draw everything ---

    // Dibuja el fondo escalado a la pantalla
    DrawTexturePro(tex_game_back2,
        { 0, 0, (float)tex_game_back2.width, (float)tex_game_back2.height },
        { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
        { 0, 0 }, 0.0f, WHITE);

    // Dibuja la bola
    DrawTexturePro(tex_ball,
        { 0, 0, (float)tex_ball.width, (float)tex_ball.height },
        { 700, 150, 50, 50 }, // Posición y tamaño
        { 25, 25 }, 0.0f, WHITE); // Origen centrado

    // Dibuja el flipper izquierdo
    DrawTexturePro(tex_boardL,
        { 0, 0, (float)tex_boardL.width, (float)tex_boardL.height },
        { 260, 960, 150, 40 }, // Posición y tamaño
        { 0, 20 }, left_flipper_rotation, WHITE); // Pivote y rotación

    // Dibuja el flipper derecho
    DrawTexturePro(tex_boardR,
        { 0, 0, (float)tex_boardR.width, (float)tex_boardR.height },
        { 540, 960, 150, 40 }, // Posición y tamaño
        { 150, 20 }, right_flipper_rotation, WHITE); // Pivote y rotación

    // Dibuja otros elementos estáticos
    DrawTexturePro(tex_crate, { 0, 0, (float)tex_crate.width, (float)tex_crate.height },
        { 150, 450, 80, 80 }, { 40, 40 }, 0.0f, WHITE);

    DrawTexturePro(tex_rick_head, { 0, 0, (float)tex_rick_head.width, (float)tex_rick_head.height },
        { 400, 250, 120, 120 }, { 60, 60 }, 0.0f, WHITE);

    DrawTexturePro(tex_wheel, { 0, 0, (float)tex_wheel.width, (float)tex_wheel.height },
        { 600, 550, 90, 90 }, { 45, 45 }, 0.0f, WHITE);

    return UPDATE_CONTINUE;
}