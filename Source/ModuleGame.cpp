#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "raylib.h"
// Necesario para las definiciones de joints
#include "box2d/box2d.h"

#define PIXEL_TO_METERS(pixel) ((float)pixel * 0.02f)
#define METERS_TO_PIXEL(meters) ((float)meters * 50.0f)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    ball = left_flipper = right_flipper = ground_anchor = NULL;
    left_joint = right_joint = NULL;
}

ModuleGame::~ModuleGame()
{
}

bool ModuleGame::Start()
{
    LOG("Loading Game assets");
    bool ret = true;

    // Carga de texturas
    tex_ball = App->renderer->Load("Assets/ball0001.png");
    tex_boardL = App->renderer->Load("Assets/boardL.png");
    tex_boardR2 = App->renderer->Load("Assets/boardR2.png");
    tex_game_back2 = App->renderer->Load("Assets/game_back2.png");
    tex_crate = App->renderer->Load("Assets/crate.png");

    if (tex_game_back2.id <= 0)
    {
        LOG("ERROR: No se pudo cargar la textura de fondo 'Assets/game_back2.png'");
    }

    // --- Creación de la Pelota ---
    ball = App->physics->CreateCircle(400, 240, 15);
    ball->body->GetFixtureList()->SetRestitution(0.8f);
    ball->body->SetType(b2_dynamicBody);

    CreateWalls();

    // --- Creación de Flippers Físicos ---
    int flipper_w = 120;
    int flipper_h = 30;

    // Posiciones de pivote en píxeles
    int left_flipper_pivot_x = 260;
    int left_flipper_pivot_y = 365;
    int right_flipper_pivot_x = 540;
    int right_flipper_pivot_y = 360;

    // 1. Crear cuerpo estático para anclar las articulaciones
    ground_anchor = App->physics->CreateRectangle(0, 0, 1, 1, b2_staticBody);

    // 2. Crear Flipper Izquierdo - posición centrada respecto al pivote
    int left_flipper_center_x = left_flipper_pivot_x + flipper_w / 2;
    int left_flipper_center_y = left_flipper_pivot_y;
    left_flipper = App->physics->CreateRectangle(left_flipper_center_x, left_flipper_center_y, flipper_w, flipper_h, b2_dynamicBody);
    left_flipper->body->SetFixedRotation(false);

    // 3. Crear Flipper Derecho - posición centrada respecto al pivote
    int right_flipper_center_x = right_flipper_pivot_x - flipper_w / 2;
    int right_flipper_center_y = right_flipper_pivot_y;
    right_flipper = App->physics->CreateRectangle(right_flipper_center_x, right_flipper_center_y, flipper_w, flipper_h, b2_dynamicBody);
    right_flipper->body->SetFixedRotation(false);

    // 4. Crear Articulación Izquierda CON conversiones correctas
    b2RevoluteJointDef left_joint_def;

    // Configurar los cuerpos
    left_joint_def.bodyA = ground_anchor->body;
    left_joint_def.bodyB = left_flipper->body;

    // Ancla en el cuerpo A (ground) - en metros desde el centro del ground_anchor
    left_joint_def.localAnchorA.Set(0, 0);

    // Ancla en el cuerpo B (flipper) - en metros desde el centro del flipper
    left_joint_def.localAnchorB.Set(PIXEL_TO_METERS(-flipper_w / 2), 0);

    // Configurar límites y motor
    left_joint_def.enableLimit = true;
    left_joint_def.lowerAngle = -15 * DEGTORAD; // Límite inferior
    left_joint_def.upperAngle = 30 * DEGTORAD;  // Límite superior
    left_joint_def.enableMotor = true;
    left_joint_def.maxMotorTorque = 1000.0f;
    left_joint_def.motorSpeed = 0.0f;

    // Posición mundial del joint
    left_joint_def.localAnchorA = ground_anchor->body->GetLocalPoint(b2Vec2(
        PIXEL_TO_METERS(left_flipper_pivot_x),
        PIXEL_TO_METERS(left_flipper_pivot_y)
    ));

    left_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_joint_def);

    // 5. Crear Articulación Derecha CON conversiones correctas
    b2RevoluteJointDef right_joint_def;

    right_joint_def.bodyA = ground_anchor->body;
    right_joint_def.bodyB = right_flipper->body;

    right_joint_def.localAnchorA.Set(0, 0);
    right_joint_def.localAnchorB.Set(PIXEL_TO_METERS(flipper_w / 2), 0);

    right_joint_def.enableLimit = true;
    right_joint_def.lowerAngle = -30 * DEGTORAD;
    right_joint_def.upperAngle = 15 * DEGTORAD;
    right_joint_def.enableMotor = true;
    right_joint_def.maxMotorTorque = 1000.0f;
    right_joint_def.motorSpeed = 0.0f;

    // Posición mundial del joint
    right_joint_def.localAnchorA = ground_anchor->body->GetLocalPoint(b2Vec2(
        PIXEL_TO_METERS(right_flipper_pivot_x),
        PIXEL_TO_METERS(right_flipper_pivot_y)
    ));

    right_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_joint_def);

    return ret;
}

void ModuleGame::CreateWalls()
{
    LOG("Creating pinball walls");

 /*   b2BodyDef wallBodyDef;
    wallBodyDef.type = b2_staticBody;
    wallBodyDef.position.Set(0, 0);
    b2Body* wallBody = App->physics->world->CreateBody(&wallBodyDef);*/

    b2BodyDef wallBodyDef;
    wallBodyDef.type = b2_staticBody;

    // Establece la posición del cuerpo en el CENTRO de la pantalla (convertido a metros)
    wallBodyDef.position.Set(
        PIXEL_TO_METERS(SCREEN_WIDTH / 2),
        PIXEL_TO_METERS(SCREEN_HEIGHT / 2)
    );

    b2Body* wallBody = App->physics->world->CreateBody(&wallBodyDef);

    // === PARED IZQUIERDA ===
// === PARED IZQUIERDA ===
    b2Vec2 leftVertices[11]; // 11 vértices
    int leftPoints[22] = {  // 22 valores (11 x 2)
        -130, -98,
        -109, -82,
        -107, 115,
        -70, 158,
        -65, 158,
        -60, 146,
        -95, 101,
        -94, -96,
        -122, -117,
        -131, -115,
        -131, -99
    };

    for (int i = 0; i < 11; i++) {
        leftVertices[i].Set(PIXEL_TO_METERS(leftPoints[i * 2]), PIXEL_TO_METERS(leftPoints[i * 2 + 1 ]));
    }

    b2ChainShape leftChain;
    leftChain.CreateLoop(leftVertices, 11);

    b2FixtureDef leftFixture;
    leftFixture.shape = &leftChain;
    leftFixture.restitution = 0.8f;
    leftFixture.friction = 0.3f;
    wallBody->CreateFixture(&leftFixture);

    // === PARED DERECHA ===
    b2Vec2 rightVertices[12]; // 12 vértices (Cambiado de 11)
    int rightPoints[24] = {  // 24 valores (12 x 2) (Cambiado de 22)
        122, -116,
        95, -97,
        95, 100,
        60, 144,
        61, 153,
        64, 157,
        70, 156,
        109, 112,
        108, -83,
        130, -99,
        131, -115,
        123, -117
    };

    for (int i = 0; i < 12; i++) { // (Cambiado de 11)
        rightVertices[i].Set(PIXEL_TO_METERS(rightPoints[i * 2]), PIXEL_TO_METERS(rightPoints[i * 2 + 1]));
    }

    b2ChainShape rightChain;
    rightChain.CreateLoop(rightVertices, 12);

    b2FixtureDef rightFixture;
    rightFixture.shape = &rightChain;
    rightFixture.restitution = 0.8f;
    rightFixture.friction = 0.3f;
    wallBody->CreateFixture(&rightFixture);

    // === PARED SUPERIOR ===
 // === PARED SUPERIOR ===
  // === PARED SUPERIOR ===
    // === PARED SUPERIOR ===
    b2Vec2 topVertices[29]; // 29 vértices (Cambiado de 30)
    int topPoints[58] = {    // 58 valores (29 x 2) (Cambiado de 60)
        -152, 240,
        -152, -163,
        -149, -169,
        -146, -174,
        -143, -178,
        -138, -182,
        -133, -186,
        -127, -190,
        -122, -192,
        -96, -192,
        -97, -202,
        -110, -217,
        -113, -221,
        -113, -226,
        -109, -230,
        108, -229,
        112, -226,
        112, -221,
        96, -192,
        118, -192,
        124, -192,
        130, -189,
        138, -184,
        143, -178,
        146, -174,
        149, -169,
        150, -162,
        151, 223,
        151, 240
        // Se ha eliminado el último punto {-152, 240} que duplicaba al primero
    };

    for (int i = 0; i < 29; i++) { // (Cambiado de 30)
        topVertices[i].Set(PIXEL_TO_METERS(topPoints[i * 2]), PIXEL_TO_METERS(topPoints[i * 2 + 1]));
    }

    b2ChainShape topChain;
    topChain.CreateLoop(topVertices, 29); // (Cambiado de 30)
    b2FixtureDef topFixture;
    topFixture.shape = &topChain;
    topFixture.restitution = 0.8f;
    topFixture.friction = 0.3f;
    wallBody->CreateFixture(&topFixture);

    LOG("Walls created successfully.");
}

bool ModuleGame::CleanUp()
{
    LOG("Unloading Game scene");

    // Limpiar texturas
    UnloadTexture(tex_ball);
    UnloadTexture(tex_boardL);
    UnloadTexture(tex_boardR2);
    UnloadTexture(tex_crate);
    UnloadTexture(tex_game_back2);

    // Los PhysBody se limpian en ModulePhysics
    // Las joints se destruyen automáticamente al destruir el mundo

    return true;
}

update_status ModuleGame::Update()
{
    // --- Controles de los flippers ---
    float motor_speed = 10.0f;

    if (IsKeyDown(KEY_LEFT))
    {
        left_joint->SetMotorSpeed(-motor_speed);
    }
    else
    {
        left_joint->SetMotorSpeed(motor_speed);
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        right_joint->SetMotorSpeed(motor_speed);
    }
    else
    {
        right_joint->SetMotorSpeed(-motor_speed);
    }

    // --- Dibujar todo ---

    // Dibujar fondo
    DrawTexturePro(tex_game_back2,
        { 0, 0, (float)tex_game_back2.width, (float)tex_game_back2.height },
        { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
        { 0, 0 }, 0.0f, WHITE);

    // Dibujar elementos del juego
    int x, y;

    // Bola
    ball->GetPhysicPosition(x, y);
    DrawTexturePro(tex_ball,
        { 0, 0, (float)tex_ball.width, (float)tex_ball.height },
        { (float)x, (float)y, 30, 30 },
        { 15, 15 },
        RADTODEG * ball->GetRotation(), WHITE);

    // Flippers
    left_flipper->GetPhysicPosition(x, y);
    DrawTexturePro(tex_boardL,
        { 0, 0, (float)tex_boardL.width, (float)tex_boardL.height },
        { (float)x, (float)y, 120, 30 },
        { 60, 15 },
        RADTODEG * left_flipper->GetRotation(), WHITE);

    right_flipper->GetPhysicPosition(x, y);
    DrawTexturePro(tex_boardR2,
        { 0, 0, (float)tex_boardR2.width, (float)tex_boardR2.height },
        { (float)x, (float)y, 120, 30 },
        { 60, 15 },
        RADTODEG * right_flipper->GetRotation(), WHITE);

    // Crate
    DrawTexturePro(tex_crate,
        { 0, 0, (float)tex_crate.width, (float)tex_crate.height },
        { 100, 200, 60, 60 },
        { 30, 30 }, 0.0f, WHITE);

    DrawTexturePro(tex_crate,
        { 0, 0, (float)tex_crate.width, (float)tex_crate.height },
        { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 60, 60 },  // <--- Esta es la línea modificada
        { 30, 30 }, 0.0f, BLACK);

 

    return UPDATE_CONTINUE;
}