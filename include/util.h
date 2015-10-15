//--------------------------------------------------------------------------------------
//
// util.h
//
//--------------------------------------------------------------------------------------
#ifndef _UTIL_H_
#define _UTIL_H_

//---------//---------//---------//---------//---------//---------//---------
// Platform
//---------//---------//---------//---------//---------//---------//---------
//---------//---------//---------//---------//---------//---------//---------
// _WINDOWS_
//---------//---------//---------//---------//---------//---------//---------
	#define _CRT_SECURE_NO_WARNINGS

	#include <windows.h>
	#include <windowsX.h>
	#include <mmsystem.h>

	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "opengl32.lib")

	#include "common.h"

	#define sscanf            sscanf_s
	#define sprintf           sprintf_s

	//---------//---------//---------//---------//---------//---------//---------
	// base DEBUG
	// http://qiita.com/saltheads/items/e1b0ab54d3d6029c9593
	//---------//---------//---------//---------//---------//---------//---------
	#define DEBUG_PRINT(...)      printf(__VA_ARGS__)
	#define GL_DEBUG              DEBUG_PRINT("%08d:%s: glErr:%08X\n", __LINE__, __FUNCTION__,  glGetError())

	//#define DEBUG_LEVEL0
	#ifdef  DEBUG_LEVEL0
		#define GL_DEBUG0         GL_DEBUG
	#else //DEBUG_LEVEL0
		#define GL_DEBUG0
	#endif

	//#define DEBUG_LEVEL1
	#ifdef  DEBUG_LEVEL1
		#define GL_DEBUG1         GL_DEBUG
	#else //DEBUG_LEVEL1
		#define GL_DEBUG1
	#endif

	//#define DEBUG_LEVEL2
	#ifdef  DEBUG_LEVEL2
		#define GL_DEBUG2         GL_DEBUG
	#else //DEBUG_LEVEL2
		#define GL_DEBUG2
	#endif
	#define GL_DEBUGEx(x)     printf("%08d:%s: glErr:%08X %08X\n", __LINE__, __FUNCTION__, __LINE__, glGetError(), x)
	//---------//---------//---------//---------//---------//---------//---------

	#define WAIT(n)           {printf("%s Wait\n", __FUNCTION__); Sleep(n); }
	#define Wait(x)           WAIT(x)

	#define GetKey(x)         (GetAsyncKeyState(x) & 0x8000)


//---------//---------//---------//---------//---------//---------//---------
// Base preference
//---------//---------//---------//---------//---------//---------//---------
#define DEFAULT_WIDTH     (1280)
#define DEFAULT_HEIGHT    (720)

//OS Function
BOOL ProcMsg();
int Init(int argc, char *argv[], void (*StartMain)(int argc, char *argv[], HDC hdc));

//MISC Function
int   random();
float frand();
void  show_fps();

//OpenGL Function
void   glSetInterval(int isinterval);
GLuint glLoadShader(const char *vsfile, const char *, const char *fsfile);

//Input Function
vec   giGetDirection(float a = 1.0);
vec   giGetButton(int a);

POINT GetMousePos();
BOOL  GetMouseLeft()  ;
BOOL  GetMouseRight() ;
int   GetWidth();
int   GetHeight();

//EVENT
void AddEvent_WM_SIZE(void (*proc)(int, int));


#endif //_UTIL_H_

