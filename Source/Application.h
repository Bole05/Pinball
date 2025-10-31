#pragma once

#include <vector>
#include "Globals.h"
#include "Module.h"

// --- Declaraciones adelantadas (Forward declarations) ---
// Le decimos al compilador que estas clases existen sin necesidad de incluir todo el .h
// Esto es VITAL para evitar errores de "inclusión circular".
class ModuleWindow;
class ModuleRender;
class ModuleGame;
class ModulePhysics;
class ModuleAudio;
class ModulePlayer; // <--- Así se declara que la clase ModulePlayer existe.

class Application
{
public:
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleGame* game;
	ModulePhysics* physics;
	ModuleAudio* audio;
	ModulePlayer* player; // <--- Ahora podemos declarar un puntero a ella.

private:
	std::vector<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	void AddModule(Module* mod);
};

// --- Variable Global ---
// Hacemos que 'App' sea una variable global accesible desde cualquier parte del código
// que incluya "Application.h".
extern Application* App;