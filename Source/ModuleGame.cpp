#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* listener)
		: body(_body), listener(listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

protected:
	PhysBody* body;
	Module* listener = nullptr;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Module* listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 25), listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 1.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;

};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Module* listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50), listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;

};

class Rick : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
	};

	Rick(ModulePhysics* physics, int _x, int _y, Module* listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(GetMouseX() - 50, GetMouseY() - 100, rick_head, 64), listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 1.0f, WHITE);
	}

private:
	Texture2D texture;
};

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = LoadTexture("Assets/wheel.png");
	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");
	paleta = LoadTexture("Assets/Instructions_pc_01.jpg");
	if (paleta.id <= 0)
	{
		LOG("Error: No se pudo cargar la textura 'paleta'.");
	}
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	// TODO: Homework - create a sensor

	return ret;
	/*return true;*/
	LOG("Loading Game");

	// Crea el contorno de la mesa
	int pinball_shape[74] = {
		2, 343, 11, 353, 12, 597, 215, 597, 215, 34, 230, 22,
		230, 9, 246, 1, 263, 1, 280, 9, 280, 22, 295, 34,
		295, 597, 500, 597, 500, 353, 508, 343, 508, 2, 2, 2
	};
	// ... después de crear la bola ...

// Flipper izquierdo
	
	int left_flipper_coords[8] = { 0, 0, 70, -20, 70, 0, 0, 10 };
	left_flipper = App->physics->CreatePolygon(150, 500, left_flipper_coords, 8);
	left_flipper_anchor = App->physics->CreateCircle(150, 500, 5, b2_staticBody);
	App->physics->CreateRevoluteJoint(left_flipper, left_flipper_anchor, 0, 0);

	// Flipper derecho (similar pero en otra posición)
	int right_flipper_coords[8] = { 0, 0, -70, -20, -70, 0, 0, 10 };
	right_flipper = App->physics->CreatePolygon(350, 500, right_flipper_coords, 8);
	right_flipper_anchor = App->physics->CreateCircle(350, 500, 5, b2_staticBody);
	App->physics->CreateRevoluteJoint(right_flipper, right_flipper_anchor, 0, 0);

	background = App->physics->CreateChain(0, 0, pinball_shape, 74, b2_staticBody);

	// Crea la bola
	ball = App->physics->CreateCircle(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 100, 10);
	ball->body->GetFixtureList()->SetRestitution(0.8f); // Haz que la bola sea rebotona

	// Carga las texturas
	tex_ball = App->renderer->Load("Assets/wheel.png");
	// Necesitarás una imagen para la bola
	if (tex_ball.id == NULL) {
		LOG("ERROR");
	}
	/*App->physics->CreateRectangleSensor(250, 200, 50, 50);*/
	
	return true;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	DrawTexture(paleta, 0, 0, WHITE);
	if (IsKeyDown(KEY_LEFT))
	{
		left_flipper->body->ApplyAngularImpulse(-20.0f, true);
	}

	// Control del flipper derecho
	if (IsKeyDown(KEY_RIGHT))
	{
		right_flipper->body->ApplyAngularImpulse(20.0f, true);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if (IsKeyPressed(KEY_ONE))
	{
		// TODO 8: Make sure to add yourself as collision callback to the circle you creates
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), this, circle));
	}

	if (IsKeyPressed(KEY_TWO))
	{
		entities.emplace_back(new Box(App->physics, GetMouseX(), GetMouseY(), this, box));
	}

	if (IsKeyPressed(KEY_THREE))
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		entities.emplace_back(new Rick(App->physics, GetMouseX(), GetMouseY(), this, rick));
	}

	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}

	// ray -----------------
	if (ray_on == true)
	{
		vec2f destination((float)(mouse.x - ray.x), (float)(mouse.y - ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	return UPDATE_CONTINUE;
}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == ball)
	{
		LOG("Collision with ball!");
		// Aquí puedes añadir lógica: si bodyB es un bumper, suma puntos, haz un sonido, etc.
		App->audio->PlayFx(bonus_fx); // Suponiendo que has cargado un efecto de sonido
	}
	else if (bodyB == ball)
	{
		LOG("Collision with ball!");
		// Lo mismo que arriba
		App->audio->PlayFx(bonus_fx);
	}
	App->audio->PlayFx(bonus_fx);
}

//ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
//{
//	
//}
//
//ModuleGame::~ModuleGame()
//{}
//
//// Load assets
//bool ModuleGame::Start()
//{
//	LOG("Loading Intro assets");
//	bool ret = true;
//	background = LoadTexture("Instructions_pc_01.jpg");
//	return ret;
//}
//
//// Load assets
//bool ModuleGame::CleanUp()
//{
//	LOG("Unloading Intro scene");
//	
//	return true;
//}
//
//// Update: draw background
//update_status ModuleGame::Update()
//{
//	DrawTexture(background, 0, 0, WHITE);
//	return UPDATE_CONTINUE;
//}
