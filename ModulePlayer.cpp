#include "Globals.h"
#include "Application.h"    // <--- INCLUIR ESTO ES LA CLAVE
#include "ModulePlayer.h"
#include "ModuleWindow.h"
#include <string>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	score = 0;
	lives = 3;
	high_score = 0;
}

ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	return true;
}

update_status ModulePlayer::Update()
{
	// Actualizar el título de la ventana para mostrar la puntuación y las vidas
	char title[256];
	// Usamos App->window porque hemos incluido "Application.h", que define la variable global App.
	sprintf_s(title, "Pinball | Score: %d | Lives: %d", score, lives);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}