//--------------------------------------------------------------------------------------
// util.h
//--------------------------------------------------------------------------------------
#ifndef _UTIL_H_
#define _UTIL_H_

//Windows
#include <windows.h>
#include <windowsX.h>
#include <mmsystem.h>

#include "common.h"

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define sscanf            sscanf_s
#define sprintf           sprintf_s
#define GL_DEBUG          printf("%08d:%s: glErr:%08X\n", __LINE__, __FUNCTION__,  glGetError())
#define GL_DEBUGEx(x)     printf("%08d:%s: glErr:%08X %08X\n", __LINE__, __FUNCTION__, __LINE__, glGetError(), x)
#define WAIT(n)           {printf("%s Wait\n", __FUNCTION__); Sleep(n); }
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

