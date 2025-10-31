#include "Globals.h"
#include "Application.h"
#include "ModuleGame.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h" // La inclusión más importante a probar

// Constructor
ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled) {}

// Destructor
ModuleGame::~ModuleGame() {}

// Start: Solo crea un círculo para ver si las físicas funcionan
bool ModuleGame::Start()
{
    LOG("Starting minimal game test...");
    // Si esta línea compila, significa que ModulePhysics y Globals están funcionando.
    App->physics->CreateCircle(100, 100, 25);
    return true;
}

// Update: Solo comprueba si el puntero al jugador es válido
update_status ModuleGame::Update()
{
    // Si esta línea compila, significa que ModulePlayer está correctamente
    // integrado en la aplicación y es accesible.
    if (App->player != nullptr)
    {
        // El test ha sido un éxito si llegamos aquí sin errores de compilación.
    }
    return UPDATE_CONTINUE;
}

// CleanUp: Vacío por ahora
bool ModuleGame::CleanUp()
{
    return true;
}

// OnCollision: Vacío por ahora
void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    // No hacemos nada en esta prueba.
}