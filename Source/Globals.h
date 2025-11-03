#pragma once

#include "raylib.h"

#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

<<<<<<< HEAD
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
=======
// Configuration -----------
#define SCREEN_WIDTH		 320
#define SCREEN_HEIGHT		  512
>>>>>>> parent of a0ed02b (iu)
#define SCREEN_SIZE				1
#define WIN_FULLSCREEN		false
#define WIN_RESIZABLE		false
#define WIN_BORDERLESS		false
<<<<<<< HEAD
>>>>>>> 73e9d37883cb79185e99d24dbab9d33025fead90
=======
>>>>>>> parent of a0ed02b (iu)
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC				true
#define TITLE "Physics 2D Playground"