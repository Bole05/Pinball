#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -50.0f // <-- He cambiado este valor

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class PhysBody
{
public:
	PhysBody() : body(NULL), listener(nullptr), width(0), height(0)
	{
	}

	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();


	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

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
};