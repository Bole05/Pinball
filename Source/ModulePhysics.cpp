#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

#include "p2Point.h"

#include <math.h>



ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = nullptr;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

//bool ModulePhysics::Start()
//{
	//LOG("Creating Physics 2D environment");
	//
	//world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	//// TODO 3: You need to make ModulePhysics class a contact listener

	//// big static circle as "ground" in the middle of the screen
	//int x = (int)(SCREEN_WIDTH / 2);
	//int y = (int)(SCREEN_HEIGHT / 1.5f);
	//int diameter = SCREEN_WIDTH / 2;

	//return true;

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);


	// Escala de conversión píxel <-> metro
	const float SCALE = 0.01f; // 1 px = 0.01 m

	// ---------------------------
	// ?? Paredes exteriores del tablero
	// ---------------------------
	b2BodyDef wallsDef;
	wallsDef.type = b2_staticBody;
	wallsDef.position.Set(0, 0);
	b2Body* walls = world->CreateBody(&wallsDef);

	b2Vec2 wallVertices[] = {
		{40 * SCALE, 310 * SCALE},   // esquina inferior izquierda
		{40 * SCALE, 60 * SCALE},    // lado izquierdo
		{120 * SCALE, 20 * SCALE},   // curva izquierda superior
		{380 * SCALE, 20 * SCALE},   // parte superior derecha
		{470 * SCALE, 60 * SCALE},   // lado derecho
		{470 * SCALE, 310 * SCALE},  // esquina inferior derecha
	};

	b2ChainShape wallShape;
	wallShape.CreateLoop(wallVertices, 6);

	b2FixtureDef wallFixture;
	wallFixture.shape = &wallShape;
	walls->CreateFixture(&wallFixture);

	// ---------------------------
	// ?? Bumpers (círculos rojos)
	// ---------------------------
	b2Vec2 bumpers[] = {
		{256 * SCALE, 150 * SCALE},
		{210 * SCALE, 170 * SCALE},
		{300 * SCALE, 170 * SCALE},
	};
	float bumperRadius = 18 * SCALE;

	for (int i = 0; i < 3; i++)
	{
		b2BodyDef bumperDef;
		bumperDef.type = b2_staticBody;
		bumperDef.position = bumpers[i];

		b2Body* bumper = world->CreateBody(&bumperDef);

		b2CircleShape bumperShape;
		bumperShape.m_radius = bumperRadius;

		b2FixtureDef bumperFix;
		bumperFix.shape = &bumperShape;
		bumperFix.restitution = 1.2f; // rebota fuerte
		bumper->CreateFixture(&bumperFix);
	}

	// ---------------------------
	// ?? Diana izquierda
	// ---------------------------
	b2BodyDef targetLeftDef;
	targetLeftDef.type = b2_staticBody;
	targetLeftDef.position.Set(150 * SCALE, 130 * SCALE);
	b2Body* targetLeft = world->CreateBody(&targetLeftDef);

	b2CircleShape targetLeftShape;
	targetLeftShape.m_radius = 15 * SCALE;

	b2FixtureDef targetLeftFix;
	targetLeftFix.shape = &targetLeftShape;
	targetLeftFix.restitution = 0.9f;
	targetLeft->CreateFixture(&targetLeftFix);

	// ---------------------------
	// ?? Diana superior (amarilla)
	// ---------------------------
	b2BodyDef targetTopDef;
	targetTopDef.type = b2_staticBody;
	targetTopDef.position.Set(256 * SCALE, 60 * SCALE);
	b2Body* targetTop = world->CreateBody(&targetTopDef);

	b2CircleShape targetTopShape;
	targetTopShape.m_radius = 25 * SCALE;

	b2FixtureDef targetTopFix;
	targetTopFix.shape = &targetTopShape;
	targetTopFix.restitution = 1.0f;
	targetTop->CreateFixture(&targetTopFix);

	// ---------------------------
	// ?? Flippers
	// ---------------------------
	// --- ELIMINADO ---
	// La creación de flippers y joints ahora está en ModuleGame::Start()


	// ---------------------------
	// ? Bola inicial
	// ---------------------------
	// --- ELIMINADO ---
	// La creación de la bola ahora está en ModuleGame::Start()

	return true;
}
//}


update_status ModulePhysics::PreUpdate()
{
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	world->Step(timeStep, velocityIterations, positionIterations);
	// -------------------------

	return UPDATE_CONTINUE;
	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type)
{
	PhysBody* pbody = new PhysBody();

	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	// TODO 4: add a pointer to PhysBody as UserData to the body
	pbody->body = b;
	pbody->width = pbody->height = radius;


	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, const int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	pbody->width = pbody->height = 0;

	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2Vec2* p = new b2Vec2[size / 2];
	for (int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);

	}

	b2PolygonShape box;
	box.Set(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	delete[] p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->GetUserData().pointer = reinterpret_cast<uintptr_t>(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{


	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();

					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{0, 0, 0, 128});
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
				}
				break;
			}


		}
	}


	return UPDATE_CONTINUE;
}




// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!


	return true;
}
void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// TODO 1: Write the code to return true in case the point
	// is inside ANY of the shapes contained by this body

	b2Fixture* fixture = body->GetFixtureList();
	while (fixture != nullptr) {
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p))
			return true;

		fixture = fixture->GetNext();
	}


	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	// TODO 2: Write code to test a ray cast between both points provided. If not hit return -1
	// if hit, fill normal_x and normal_y and return the distance between x1,y1 and it's colliding point

	int ret = -1;

	b2RayCastOutput output;
	b2RayCastInput input;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0)) {

			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return (int)(output.fraction * dist);
		}
	}

	return ret;
}

// TODO 3
// TODO 7: Call the listeners that are not NULL
void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* a = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	PhysBody* b = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (a && a->listener) {
		a->listener->OnCollision(a, b);
	}

	if (b && b->listener) {
		b->listener->OnCollision(b, a);
	}
}
// Añade esta función completa al final del archivo
void ModulePhysics::CreateRevoluteJoint(PhysBody* bodyA, PhysBody* bodyB, int anchor_x, int anchor_y)
{
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = bodyA->body;
	revoluteJointDef.bodyB = bodyB->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(anchor_x), PIXEL_TO_METERS(anchor_y));
	revoluteJointDef.localAnchorB.Set(0, 0); // La bisagra estará en el origen del flipper
	revoluteJointDef.enableLimit = true;
	revoluteJointDef.lowerAngle = -25 * DEGTORAD;
	revoluteJointDef.upperAngle = 25 * DEGTORAD;

	world->CreateJoint(&revoluteJointDef);
}