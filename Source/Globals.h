#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
void log(const char file[], int line, const char* format, ...);

// --- Definiciones del estado de la aplicación ---
enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

<<<<<<< HEAD
// --- Constantes de configuración ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 900
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
=======
// Configuration -----------
#define SCREEN_WIDTH		 800
#define SCREEN_HEIGHT		  480
#define SCREEN_SIZE				1
#define WIN_FULLSCREEN		false
#define WIN_RESIZABLE		false
#define WIN_BORDERLESS		false
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Pinball Game"

// --- Macros para Físicas (¡MUY IMPORTANTE!) ---
#define METERS_TO_PIXELS(m) ((int) floor(m * 50.0f))
#define PIXEL_TO_METERS(p) (((float) p) / 50.0f)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f