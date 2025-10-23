#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"

// Necesario para las definiciones de joints
#include "box2d/box2d.h"


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    // Inicializa los punteros a null
    ball = left_flipper = right_flipper = ground_anchor = NULL;
    left_joint = right_joint = NULL;
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
    LOG("Loading Game assets");
    bool ret = true;

    // Carga de texturas (sin cambios)
    tex_ball = App->renderer->Load("Assets/ball0001.png");
    tex_boardL = App->renderer->Load("Assets/boardL.png");
    tex_boardR = App->renderer->Load("Assets/boardR.png");

    tex_game_back2 = App->renderer->Load("Assets/game_back2.png");

    if (tex_game_back2.id <= 0)
    {
        LOG("ERROR: No se pudo cargar la textura de fondo 'Assets/game_back.jpg'");
    }

    // --- Creación de la Pelota ---
    ball = App->physics->CreateCircle(400, 240, 15);
    ball->body->GetFixtureList()->SetRestitution(0.8f);

    // --- Creación de Flippers Físicos ---

    // Propiedades de los flippers (basadas en el DrawTexturePro)
    int flipper_w = 120;
    int flipper_h = 30;
    int left_flipper_pivot_x = 280;
    int left_flipper_pivot_y = 350 + 15; // Posición Y + Origen Y
    int right_flipper_pivot_x = 520; // Posición X + Origen X
    int right_flipper_pivot_y = 350 + 15; // Posición Y + Origen Y

    // 1. Crear un cuerpo estático para anclar las articulaciones
    ground_anchor = App->physics->CreateRectangle(0, 0, 1, 1, b2_staticBody);

    // 2. Crear Flipper Izquierdo
    // Creamos el rectángulo centrado
    int left_flipper_center_x = left_flipper_pivot_x + flipper_w / 2;
    int left_flipper_center_y = left_flipper_pivot_y;
    left_flipper = App->physics->CreateRectangle(left_flipper_center_x, left_flipper_center_y, flipper_w, flipper_h, b2_dynamicBody);

    // 3. Crear Flipper Derecho
    // Creamos el rectángulo centrado
    int right_flipper_center_x = right_flipper_pivot_x - flipper_w / 2;
    int right_flipper_center_y = right_flipper_pivot_y;
    right_flipper = App->physics->CreateRectangle(right_flipper_center_x, right_flipper_center_y, flipper_w, flipper_h, b2_dynamicBody);

    // 4. Crear Articulación (Joint) Izquierda
    b2RevoluteJointDef left_joint_def;
    left_joint_def.bodyA = ground_anchor->body;
    left_joint_def.bodyB = left_flipper->body;

    // Ancla en el mundo (posición del pivote)
    left_joint_def.localAnchorA.Set(PIXEL_TO_METERS(left_flipper_pivot_x), PIXEL_TO_METERS(left_flipper_pivot_y));
    // Ancla relativa al centro del flipper
    left_joint_def.localAnchorB.Set(PIXEL_TO_METERS(-flipper_w / 2), PIXEL_TO_METERS(0));

    left_joint_def.enableLimit = true;
    left_joint_def.lowerAngle = -15 * DEGTORAD; // Límite "pulsado"
    left_joint_def.upperAngle = 30 * DEGTORAD;  // Límite "reposo"
    left_joint_def.enableMotor = true;
    left_joint_def.maxMotorTorque = 1000.0f;
    left_joint_def.motorSpeed = 0.0f;

    left_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_joint_def);

    // 5. Crear Articulación (Joint) Derecha
    b2RevoluteJointDef right_joint_def;
    right_joint_def.bodyA = ground_anchor->body;
    right_joint_def.bodyB = right_flipper->body;

    // Ancla en el mundo (posición del pivote)
    right_joint_def.localAnchorA.Set(PIXEL_TO_METERS(right_flipper_pivot_x), PIXEL_TO_METERS(right_flipper_pivot_y));
    // Ancla relativa al centro del flipper
    right_joint_def.localAnchorB.Set(PIXEL_TO_METERS(flipper_w / 2), PIXEL_TO_METERS(0));

    right_joint_def.enableLimit = true;
    right_joint_def.lowerAngle = -30 * DEGTORAD; // Límite "reposo"
    right_joint_def.upperAngle = 15 * DEGTORAD;   // Límite "pulsado"
    right_joint_def.enableMotor = true;
    right_joint_def.maxMotorTorque = 1000.0f;
    right_joint_def.motorSpeed = 0.0f;

    right_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_joint_def);

    // // Rotaciones iniciales (ya no se necesitan)
    // left_flipper_rotation = 30.0f;
    // right_flipper_rotation = -30.0f;

    return ret;
}

// Unload assets
bool ModuleGame::CleanUp()
{
    LOG("Unloading Game scene");
    UnloadTexture(tex_ball);
    UnloadTexture(tex_boardL);
    UnloadTexture(tex_boardR);
    UnloadTexture(tex_crate);
    UnloadTexture(tex_game_back2);
    UnloadTexture(tex_rick_head);
    UnloadTexture(tex_wheel);

    // Los PhysBody se limpian en el CleanUp de ModulePhysics
    // pero las joints deben destruirse si es necesario (aunque al destruir el mundo se destruyen)

    return true;
}

// Update game logic and draw
update_status ModuleGame::Update()
{
    // --- Controles de los flippers (con física) ---
    float motor_speed = 20.0f; // Velocidad de rotación del flipper

    if (IsKeyDown(KEY_LEFT))
    {
        // Mover hacia el límite inferior (-15 grados)
        left_joint->SetMotorSpeed(-motor_speed);
    }
    else
    {
        // Mover hacia el límite superior (30 grados)
        left_joint->SetMotorSpeed(motor_speed);
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        // Mover hacia el límite superior (15 grados)
        right_joint->SetMotorSpeed(motor_speed);
    }
    else
    {
        // Mover hacia el límite inferior (-30 grados)
        right_joint->SetMotorSpeed(-motor_speed);
    }

    // --- Dibuja todo con los nuevos tamaños y posiciones ---

    // Dibuja el fondo escalado a la pantalla
    DrawTexturePro(tex_game_back2,
        { 0, 0, (float)tex_game_back2.width, (float)tex_game_back2.height },
        { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
        { 0, 0 }, 0.0f, WHITE);


    // --- Dibuja la bola (obteniendo datos de PhysBody) ---
    int x, y;
    ball->GetPhysicPosition(x, y);
    DrawTexturePro(tex_ball,
        { 0, 0, (float)tex_ball.width, (float)tex_ball.height },
        { (float)x, (float)y, 30, 30 }, // Usa las coordenadas del cuerpo físico
        { 15, 15 },                     // Origen centrado
        RADTODEG * ball->GetRotation(), // Usa la rotación del cuerpo físico
        WHITE);


    // --- Dibuja el flipper izquierdo (obteniendo datos de PhysBody) ---
    left_flipper->GetPhysicPosition(x, y);
    DrawTexturePro(tex_boardL,
        { 0, 0, (float)tex_boardL.width, (float)tex_boardL.height },
        { (float)x, (float)y, 120, 30 }, // Usa las coordenadas del cuerpo físico
        { 60, 15 }, // Origen en el centro (120/2, 30/2)
        RADTODEG * left_flipper->GetRotation(), // Usa la rotación del cuerpo físico
        WHITE);


    // --- Dibuja el flipper derecho (obteniendo datos de PhysBody) ---
    right_flipper->GetPhysicPosition(x, y);
    DrawTexturePro(tex_boardR,
        { 0, 0, (float)tex_boardR.width, (float)tex_boardR.height },
        { (float)x, (float)y, 120, 30 }, // Usa las coordenadas del cuerpo físico
        { 60, 15 }, // Origen en el centro (120/2, 30/2)
        RADTODEG * right_flipper->GetRotation(), // Usa la rotación del cuerpo físico
        WHITE);

    // Dibuja otros elementos estáticos (sin cambios)
    DrawTexturePro(tex_crate, { 0, 0, (float)tex_crate.width, (float)tex_crate.height },
        { 100, 200, 60, 60 }, { 30, 30 }, 0.0f, WHITE);

    DrawTexturePro(tex_rick_head, { 0, 0, (float)tex_rick_head.width, (float)tex_rick_head.height },
        { 400, 150, 100, 100 }, { 50, 50 }, 0.0f, WHITE);

    DrawTexturePro(tex_wheel, { 0, 0, (float)tex_wheel.width, (float)tex_wheel.height },
        { 650, 250, 70, 70 }, { 35, 35 }, 0.0f, WHITE);

    return UPDATE_CONTINUE;
}