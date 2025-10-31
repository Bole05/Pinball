#pragma once
#include "Module.h"
#include "box2d/box2d.h"

// --- Clase PhysBody ---
// La definimos aquí para que todos los que incluyan ModulePhysics.h la conozcan.
class PhysBody
{
public:
	// Es buena práctica inicializar las variables en el constructor
	PhysBody() : body(nullptr), listener(nullptr), width(0), height(0) {}
	~PhysBody() {}

	// Métodos que tu código necesita
	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// --- Módulo de Físicas ---
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// Funciones para crear cuerpos
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_staticBody);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type = b2_staticBody);

	// Listener de contactos de Box2D
	void BeginContact(b2Contact* contact);

private:
	b2World* world;
};