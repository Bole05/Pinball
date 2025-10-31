#include "Globals.h"
#include "Application.h"
#include "ModuleGame.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h" // La inclusi�n m�s importante a probar

// Constructor
ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled) {}

// Destructor
ModuleGame::~ModuleGame() {}

// Start: Solo crea un c�rculo para ver si las f�sicas funcionan
bool ModuleGame::Start()
{
    LOG("Starting minimal game test...");
    // Si esta l�nea compila, significa que ModulePhysics y Globals est�n funcionando.
    App->physics->CreateCircle(100, 100, 25);
    return true;
}

// Update: Solo comprueba si el puntero al jugador es v�lido
update_status ModuleGame::Update()
{
    // Si esta l�nea compila, significa que ModulePlayer est� correctamente
    // integrado en la aplicaci�n y es accesible.
    if (App->player != nullptr)
    {
        // El test ha sido un �xito si llegamos aqu� sin errores de compilaci�n.
    }
    return UPDATE_CONTINUE;
}

// CleanUp: Vac�o por ahora
bool ModuleGame::CleanUp()
{
    return true;
}

// OnCollision: Vac�o por ahora
void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    // No hacemos nada en esta prueba.
}