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

    // Carga de texturas (sin cambios)
    tex_ball = App->renderer->Load("Assets/ball0001.png");
    tex_boardL = App->renderer->Load("Assets/boardL.png");
    tex_boardR = App->renderer->Load("Assets/boardR.png");

    tex_game_back2 = App->renderer->Load("Assets/game_back2.png");


    if (tex_game_back2.id <= 0)
    {
        LOG("ERROR: No se pudo cargar la textura de fondo 'Assets/game_back.jpg'");
    }

    // Rotaciones iniciales de los flippers
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
    // Controles de los flippers (sin cambios)
    if (IsKeyDown(KEY_LEFT))
    {
        left_flipper_rotation = -35.0f;
    }
    else
    {
        left_flipper_rotation = 20.0f;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        right_flipper_rotation = 35.0f;
    }
    else
    {
        right_flipper_rotation = -20.0f;
    }

    // --- Dibuja todo con los nuevos tamaños y posiciones ---

    // Dibuja el fondo escalado a la pantalla
    DrawTexturePro(tex_game_back2,
        { 0, 0, (float)tex_game_back2.width, (float)tex_game_back2.height },
        { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
        { 0, 0 }, 0.0f, WHITE);

    // --- CORRECCIÓN AQUÍ ---
    // Dibuja la bola (más pequeña)
    DrawTexturePro(tex_ball,
        { 0, 0, (float)tex_ball.width, (float)tex_ball.height },
        { 750, 100, 30, 30 }, // Nueva posición y tamaño
        { 15, 15 }, 0.0f, WHITE); // Origen centrado

    // --- CORRECCIÓN AQUÍ ---
    // Dibuja el flipper izquierdo (más corto y reposicionado)
    DrawTexturePro(tex_boardL,
        { 0, 0, (float)tex_boardL.width, (float)tex_boardL.height },
        { 280, 400, 120, 30 }, // Nueva posición y tamaño
        { 0, 15 }, left_flipper_rotation, WHITE); // Pivote y rotación

    // --- CORRECCIÓN AQUÍ ---
    // Dibuja el flipper derecho (más corto y reposicionado)
    DrawTexturePro(tex_boardR,
        { 0, 0, (float)tex_boardR.width, (float)tex_boardR.height },
        { 520, 400, 120, 30 }, // Nueva posición y tamaño
        { 120, 15 }, right_flipper_rotation, WHITE); // Pivote y rotación

    // Dibuja otros elementos estáticos (reposicionados para la nueva vista)
    DrawTexturePro(tex_crate, { 0, 0, (float)tex_crate.width, (float)tex_crate.height },
        { 100, 200, 60, 60 }, { 30, 30 }, 0.0f, WHITE);

    DrawTexturePro(tex_rick_head, { 0, 0, (float)tex_rick_head.width, (float)tex_rick_head.height },
        { 400, 150, 100, 100 }, { 50, 50 }, 0.0f, WHITE);

    DrawTexturePro(tex_wheel, { 0, 0, (float)tex_wheel.width, (float)tex_wheel.height },
        { 650, 250, 70, 70 }, { 35, 35 }, 0.0f, WHITE);

    return UPDATE_CONTINUE;
}