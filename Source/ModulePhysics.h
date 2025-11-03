#pragma once
<<<<<<< HEAD
#include "Module.h"
<<<<<<< HEAD
#include "box2d/box2d.h"
=======
#include "Globals.h"

#include "box2d\box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -15.0f // <-- He cambiado este valor

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
>>>>>>> parent of a0ed02b (iu)

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class PhysBody
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
	// Es buena práctica inicializar las variables en el constructor
	PhysBody() : body(nullptr), listener(nullptr), width(0), height(0) {}
	~PhysBody() {}
=======
	PhysBody() : body(NULL), listener(nullptr), width(0), height(0)
	{
	}
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
	PhysBody() : body(NULL), listener(nullptr)
	{
	}
>>>>>>> parent of a0ed02b (iu)

	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	// TODO 6: Add a pointer to a module that might want to listen to a collision from this body
	Module* listener;
};
<<<<<<< HEAD

<<<<<<< HEAD
// --- Módulo de Físicas ---
=======
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
class ModulePhysics : public Module, public b2ContactListener
=======
// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
>>>>>>> parent of a0ed02b (iu)
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
	
>>>>>>> parent of a0ed02b (iu)
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

<<<<<<< HEAD
<<<<<<< HEAD
	// Funciones para crear cuerpos
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_staticBody);
=======
	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
>>>>>>> parent of a0ed02b (iu)
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, const int* points, int size);

	void BeginContact(b2Contact* contact) override;

private:

	bool debug;
	b2World* world;
=======
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_dynamicBody);
	PhysBody* CreateChain(int x, int y, const int* points, int size, b2BodyType type = b2_staticBody);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType type = b2_dynamicBody);

	void BeginContact(b2Contact* contact) override;
	void CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, int anchor_x, int anchor_y);

public: // <--- CAMBIO AQUÍ
	b2World* world;

private:

	bool debug;
	// b2World* world; // <-- MOVIDO A PUBLIC
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
};