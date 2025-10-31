#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"

// ... (todo tu código existente de ModulePhysics.cpp va aquí) ...

// AÑADE ESTA FUNCIÓN AL FINAL DE TU ARCHIVO ModulePhysics.cpp
PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
    b2BodyDef body;
    body.type = b2_staticBody;
    body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

    b2Body* b = world->CreateBody(&body);

    b2PolygonShape box;
    box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

    b2FixtureDef fixture;
    fixture.shape = &box;
    fixture.density = 1.0f;
    fixture.isSensor = true; // Esto lo convierte en un sensor

    b->CreateFixture(&fixture);

    PhysBody* pbody = new PhysBody();
    pbody->body = b;
    b->SetUserData(pbody);
    pbody->width = width;
    pbody->height = height;

    return pbody;
}