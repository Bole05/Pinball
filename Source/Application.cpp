#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"


Application::Application()
{
	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	physics = new ModulePhysics(this);
	audio = new ModuleAudio(this);

	game = new ModuleGame(this);

	// El orden de las llamadas es muy importante.
	// Los módulos se inicializarán, iniciarán y actualizarán en este orden.
	// Se limpiarán en orden inverso.

	// Módulos principales
	AddModule(window);
	AddModule(physics);
	AddModule(audio);

	AddModule(renderer);

	// Escenas
	AddModule(game);
}

Application::~Application()
{
	for (std::vector<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	for (Module* item : list_modules)
	{
		ret = item->Init();
		if (ret == false) break;
	}

	for (Module* item : list_modules)
	{
		ret = item->Start();
		if (ret == false) break;
	}

	return ret;
}

// Llamar a los PreUpdate, Update y PostUpdate de todos los módulos
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (Module* item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != UPDATE_CONTINUE) break;
	}

	for (Module* item : list_modules)
	{
		ret = item->Update();
		if (ret != UPDATE_CONTINUE) break;
	}

	for (Module* item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != UPDATE_CONTINUE) break;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::vector<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}