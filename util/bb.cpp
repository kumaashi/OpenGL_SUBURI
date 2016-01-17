
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <windows.h>
#include <windowsx.h>


#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class WinApp {
	const char *AppName;
	BOOL Windowed  ;
	HWND hWnd;
	static int  Width ;
	static int  Height;
	static int  xPos  ;
	static int  yPos  ;
	static BOOL mLeft ;
	static BOOL mRight;
	
	static LRESULT CALLBACK WinApp::MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	WinApp(const char *name, int width, int height)
	{
		AppName        = "dummy";
		Width          = 0;
		Height         = 0;
		hWnd           = NULL;
		Windowed       = TRUE;
		xPos           = 0;
		yPos           = 0;
		mLeft          = FALSE;
		mRight         = FALSE;
		Run(name, width, height);
	}
	
	
	int GetWidth()
	{
		return Width;
	}

	int GetHeight()
	{
		return Height;
	}
	
	

	~WinApp()
	
	{
		Term();
	}
	
	void Term()
	{
	}

	int Run(const char *name, int width, int height)
	{
		AppName = name;
		Width = width;
		Height = height;

		printf("%s : %s -> %d %d : ", __FUNCTION__, AppName, Width, Height);

		static WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
			WinApp::MainWindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
			LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH),
			NULL, AppName, NULL
		};

		wcex.hInstance = GetModuleHandle(NULL);
		DWORD style   = Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
		DWORD styleex = WS_EX_APPWINDOW | (Windowed ? WS_EX_WINDOWEDGE : 0);

		RECT   rc;
		SetRect(&rc, 0, 0, Width, Height);
		AdjustWindowRectEx(&rc, style, FALSE, styleex);
		rc.right  -= rc.left;
		rc.bottom -= rc.top;

		if (!RegisterClassEx(&wcex))
		{
			printf("%s : failed RegisterClassEx\n", __FUNCTION__);
			return 0;
		}

		hWnd = CreateWindowEx(
				   styleex, AppName, AppName, style,
				   (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2,
				   (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
				   rc.right, rc.bottom, 0, 0, wcex.hInstance, (void *)this);

		if(!hWnd)
		{
			printf("%s : failed CreateWindowEx\n", __FUNCTION__);
			return 0;
		}

		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		printf(" Done.\n");

		return 0;
	}

	int ProcMsg()
	{
		if(hWnd == NULL)
		{
			return 0;
		}

		MSG msg;
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				return 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return 1;
	}

	int ShowFps()
	{
		static DWORD	last	 = timeGetTime();
		static DWORD	frames   = 0;
		static char	 buf[256] = "";

		DWORD		   current;
		current = timeGetTime();
		frames++;

		if(1000 <= current - last)
		{
			float dt  = (float)(current - last) / 1000.0f;
			float fps = (float)frames / dt;
			last	  = current;
			frames	= 0;
			sprintf(buf, "%.02f fps", fps);
			printf("%s\n", buf);
		}

		return 0;
	}
	
	
	HWND GetHwnd()
	{
		if(hWnd == NULL)
		{
			printf("Warning HWND is nil\n");
		}
		return hWnd;
	}
};


int  WinApp::Width  = 0;
int  WinApp::Height = 0;
int  WinApp::xPos   = 0;
int  WinApp::yPos   = 0;
BOOL WinApp::mLeft  = 0;
BOOL WinApp::mRight = 0;
	
LRESULT CALLBACK WinApp::MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int temp = wParam & 0xFFF0;
	switch(msg)
	{
	case WM_MOUSEMOVE:
		{
			xPos = GET_X_LPARAM(lParam);
			yPos = GET_Y_LPARAM(lParam);
		}
		break;

	case WM_LBUTTONDOWN: mLeft  = TRUE;  break;
	case WM_LBUTTONUP:   mLeft  = FALSE; break;
	case WM_RBUTTONDOWN: mRight = TRUE;  break;
	case WM_RBUTTONUP:   mRight = FALSE; break;
	case WM_SYSCOMMAND:
		if(temp == SC_MONITORPOWER || temp == SC_SCREENSAVE)
		{
			return 0;
		}
		break;

	case WM_IME_SETCONTEXT:
		lParam &= ~ISC_SHOWUIALL;
		break;

	case WM_SIZE:
		Width  = LOWORD(lParam);
		Height = HIWORD(lParam);
		/*
		if(ProcHandleResize) {
			ProcHandleResize(Width, Height);
		}
		*/
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;

	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE) PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////
//
// GL Parts
//
////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------------------
//
// OpenGL Function
//
//--------------------------------------------------------------------------------------
HDC                                     hDC                         = NULL;
PFNGLACTIVETEXTUREPROC                  glActiveTexture             = NULL;
PFNGLATTACHSHADERPROC                   glAttachShader              = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC         glBeginTransformFeedback    = NULL;
PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation        = NULL;
PFNGLBINDBUFFERBASEPROC                 glBindBufferBase            = NULL;
PFNGLBINDBUFFERPROC                     glBindBuffer                = NULL;
PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer           = NULL;
PFNGLBINDIMAGETEXTUREPROC               glBindImageTexture          = NULL;
PFNGLBINDRENDERBUFFERPROC               glBindRenderBuffer          = NULL;
PFNGLBINDVERTEXARRAYPROC                glBindVertexArray           = NULL;
PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer           = NULL;
PFNGLBUFFERDATAPROC                     glBufferData                = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC         glCheckFramebufferStatus    = NULL;
PFNGLCOMPILESHADERPROC                  glCompileShader             = NULL;
PFNGLCREATEPROGRAMPROC                  glCreateProgram             = NULL;
PFNGLCREATESHADERPROC                   glCreateShader              = NULL;
PFNGLDELETEBUFFERSPROC                  glDeleteBuffers             = NULL;
PFNGLDELETEFRAMEBUFFERSPROC             glDeleteFramebuffers        = NULL;
PFNGLDELETEPROGRAMPROC                  glDeleteProgram             = NULL;
PFNGLDELETERENDERBUFFERSPROC            glDeleteRenderbuffers       = NULL;
PFNGLDELETESHADERPROC                   glDeleteShader              = NULL;
PFNGLDELETEVERTEXARRAYSPROC             glDeleteVertexArrays        = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC       glDisableVertexAttribArray  = NULL;
PFNGLDRAWBUFFERSPROC                    glDrawBuffers               = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC        glEnableVertexAttribArray   = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC           glEndTransformFeedback      = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC        glFramebufferRenderbuffer   = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC           glFramebufferTexture2D      = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC             glFramebufferTexture        = NULL;
PFNGLGENBUFFERSPROC                     glGenBuffers                = NULL;
PFNGLGENERATEMIPMAPPROC                 glGenerateMipmap            = NULL;
PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers           = NULL;
PFNGLGENRENDERBUFFERSPROC               glGenRenderbuffers          = NULL;
PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays           = NULL;
PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation         = NULL;
PFNGLGETBUFFERSUBDATAPROC               glGetBufferSubData          = NULL;
PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog         = NULL;
PFNGLGETPROGRAMIVPROC                   glGetProgramiv              = NULL;
PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog          = NULL;
PFNGLGETSHADERIVPROC                    glGetShaderiv               = NULL;
PFNGLGETUNIFORMLOCATIONPROC             glGetUniformLocation        = NULL;
PFNGLLINKPROGRAMPROC                    glLinkProgram               = NULL;
PFNGLPROGRAMUNIFORM1IPROC               glProgramUniform1i          = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = NULL;
PFNGLRENDERBUFFERSTORAGEPROC            glRenderBufferStorage       = NULL;
PFNGLRENDERBUFFERSTORAGEPROC            glRenderbufferStorage       = NULL;
PFNGLSHADERSOURCEPROC                   glShaderSource              = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC          glTexImage2DMultisample     = NULL;
PFNGLTEXIMAGE3DPROC                     glTexImage3D                = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC      glTransformFeedbackVaryings = NULL;
PFNGLUNIFORM1FPROC                      glUniform1f                 = NULL;
PFNGLUNIFORM1IPROC                      glUniform1i                 = NULL;
PFNGLUNIFORM4FVPROC                     glUniform4fv                = NULL;
PFNGLUNIFORMMATRIX4FVPROC               glUniformMatrix4fv          = NULL;
PFNGLUSEPROGRAMPROC                     glUseProgram                = NULL;
PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer       = NULL;
PFNGLDISPATCHCOMPUTEPROC                glDispatchCompute           = NULL;
PFNGLBINDFRAGDATALOCATIONPROC           glBindFragDataLocation      = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC            glUniformBlockBinding       = NULL;

void glInitFunc()
{
	glActiveTexture             = (PFNGLACTIVETEXTUREPROC             )wglGetProcAddress("glActiveTexture");
	glAttachShader              = (PFNGLATTACHSHADERPROC              )wglGetProcAddress("glAttachShader");
	glBeginTransformFeedback    = (PFNGLBEGINTRANSFORMFEEDBACKPROC    )wglGetProcAddress("glBeginTransformFeedback");
	glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC        )wglGetProcAddress("glBindAttribLocation");
	glBindBuffer                = (PFNGLBINDBUFFERPROC                )wglGetProcAddress("glBindBuffer");
	glBindBufferBase            = (PFNGLBINDBUFFERBASEPROC            )wglGetProcAddress("glBindBufferBase");
	glBindFramebuffer           = (PFNGLBINDFRAMEBUFFERPROC           )wglGetProcAddress("glBindFramebuffer");
	glBindImageTexture          = (PFNGLBINDIMAGETEXTUREPROC          )wglGetProcAddress("glBindImageTexture");
	glBindRenderBuffer          = (PFNGLBINDRENDERBUFFERPROC          )wglGetProcAddress("glBindRenderbuffer");
	glBindVertexArray           = (PFNGLBINDVERTEXARRAYPROC           )wglGetProcAddress("glBindVertexArray");
	glBlitFramebuffer           = (PFNGLBLITFRAMEBUFFERPROC           )wglGetProcAddress("glBlitFramebuffer");
	glBufferData                = (PFNGLBUFFERDATAPROC                )wglGetProcAddress("glBufferData");
	glCheckFramebufferStatus    = (PFNGLCHECKFRAMEBUFFERSTATUSPROC    )wglGetProcAddress("glCheckFramebufferStatus");
	glCompileShader             = (PFNGLCOMPILESHADERPROC             )wglGetProcAddress("glCompileShader");
	glCreateProgram             = (PFNGLCREATEPROGRAMPROC             )wglGetProcAddress("glCreateProgram");
	glCreateShader              = (PFNGLCREATESHADERPROC              )wglGetProcAddress("glCreateShader");
	glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC             )wglGetProcAddress("glDeleteBuffers");
	glDeleteFramebuffers        = (PFNGLDELETEFRAMEBUFFERSPROC        )wglGetProcAddress("glDeleteFramebuffers");
	glDeleteProgram             = (PFNGLDELETEPROGRAMPROC             )wglGetProcAddress("glDeleteProgram");
	glDeleteRenderbuffers       = (PFNGLDELETERENDERBUFFERSPROC       )wglGetProcAddress("glDeleteRenderbuffers");
	glDeleteShader              = (PFNGLDELETESHADERPROC              )wglGetProcAddress("glDeleteShader");
	glDeleteVertexArrays        = (PFNGLDELETEVERTEXARRAYSPROC        )wglGetProcAddress("glDeleteVertexArrays");
	glDisableVertexAttribArray  = (PFNGLDISABLEVERTEXATTRIBARRAYPROC  )wglGetProcAddress("glDisableVertexAttribArray");
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC               )wglGetProcAddress("glDrawBuffers");
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC               )wglGetProcAddress("glDrawBuffers");
	glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC   )wglGetProcAddress("glEnableVertexAttribArray");
	glEndTransformFeedback      = (PFNGLENDTRANSFORMFEEDBACKPROC      )wglGetProcAddress("glEndTransformFeedback");
	glFramebufferRenderbuffer   = (PFNGLFRAMEBUFFERRENDERBUFFERPROC   )wglGetProcAddress("glFramebufferRenderbuffer");
	glFramebufferTexture        = (PFNGLFRAMEBUFFERTEXTUREPROC        )wglGetProcAddress("glFramebufferTexture");
	glFramebufferTexture2D      = (PFNGLFRAMEBUFFERTEXTURE2DPROC      )wglGetProcAddress("glFramebufferTexture2D");
	glGenBuffers                = (PFNGLGENBUFFERSPROC                )wglGetProcAddress("glGenBuffers");
	glGenFramebuffers           = (PFNGLGENFRAMEBUFFERSPROC           )wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers          = (PFNGLGENRENDERBUFFERSPROC          )wglGetProcAddress("glGenRenderbuffers");
	glGenVertexArrays           = (PFNGLGENVERTEXARRAYSPROC           )wglGetProcAddress("glGenVertexArrays");
	glGenerateMipmap            = (PFNGLGENERATEMIPMAPPROC            )wglGetProcAddress("glGenerateMipmap");
	glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC         )wglGetProcAddress("glGetAttribLocation");
	glGetBufferSubData          = (PFNGLGETBUFFERSUBDATAPROC          )wglGetProcAddress("glGetBufferSubData");
	glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC         )wglGetProcAddress("glGetProgramInfoLog");
	glGetProgramiv              = (PFNGLGETPROGRAMIVPROC              )wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC          )wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv               = (PFNGLGETSHADERIVPROC               )wglGetProcAddress("glGetShaderiv");
	glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC        )wglGetProcAddress("glGetUniformLocation");
	glLinkProgram               = (PFNGLLINKPROGRAMPROC               )wglGetProcAddress("glLinkProgram");
	glProgramUniform1i          = (PFNGLPROGRAMUNIFORM1IPROC          )wglGetProcAddress("glProgramUniform1i");
	glRenderBufferStorage       = (PFNGLRENDERBUFFERSTORAGEPROC       )wglGetProcAddress("glRenderbufferStorage");
	glRenderbufferStorage       = (PFNGLRENDERBUFFERSTORAGEPROC       )wglGetProcAddress("glRenderbufferStorage");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	glShaderSource              = (PFNGLSHADERSOURCEPROC              )wglGetProcAddress("glShaderSource");
	glTexImage2DMultisample     = (PFNGLTEXIMAGE2DMULTISAMPLEPROC     )wglGetProcAddress("glTexImage2DMultisample");
	glTexImage3D                = (PFNGLTEXIMAGE3DPROC                )wglGetProcAddress("glTexImage3D");
	glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC )wglGetProcAddress("glTransformFeedbackVaryings");
	glUniform1f                 = (PFNGLUNIFORM1FPROC                 )wglGetProcAddress("glUniform1f");
	glUniform1i                 = (PFNGLUNIFORM1IPROC                 )wglGetProcAddress("glUniform1i");
	glUniform4fv                = (PFNGLUNIFORM4FVPROC                )wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC          )wglGetProcAddress("glUniformMatrix4fv");
	glUseProgram                = (PFNGLUSEPROGRAMPROC                )wglGetProcAddress("glUseProgram");
	glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC       )wglGetProcAddress("glVertexAttribPointer");
	glDispatchCompute           = (PFNGLDISPATCHCOMPUTEPROC           )wglGetProcAddress("glDispatchCompute");
	glBindFragDataLocation      = (PFNGLBINDFRAGDATALOCATIONPROC      )wglGetProcAddress("glBindFragDataLocation");
	glUniformBlockBinding       = (PFNGLUNIFORMBLOCKBINDINGPROC       )wglGetProcAddress("glUniformBlockBinding");
	
	
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);
	glActiveTexture(GL_TEXTURE2);
	glActiveTexture(GL_TEXTURE3);
	glEnable( GL_MULTISAMPLE );
}

int InitOpenGL(HWND hWnd)
{
	if(hWnd == NULL) {
		printf("hWnd is NULL");
		return 0;
	}
	hDC = GetDC(hWnd);
	static const PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32
	};

	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	wglMakeCurrent(hDC, wglCreateContext(hDC));
	glInitFunc();
	return 0;
}

struct Box {
	bool mark;
	float min[4], max[4];
	void Set(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax, float rmin = 0, float rmax = 0) {
		min[0] = xmin; min[1] = ymin; min[2] = zmin;
		max[0] = xmax; max[1] = ymax; max[2] = zmax;
		min[3] = 0;
		max[3] = 0; 
	}
	void Disp() {
		printf("hit=%d, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f\n", mark, min[0], min[1], min[2], max[0], max[1], max[2]);
	}
	Box() {
		Reset();
		//printf("%s : sizeof=%d\n", __FUNCTION__, sizeof(Box));
	}

	Box(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) {
		Reset();
		min[0] = xmin; min[1] = ymin; min[2] = zmin;
		max[0] = xmax; max[1] = ymax; max[2] = zmax;
	}

	void Mark(bool a) {
		if(!mark) mark = a;
	}

	void Reset() {
		mark = 0;
		Set(0, 0, 0, 0, 0, 0);
	}

	bool Hit(Box &a) {
		bool ret = true;
		for(int i = 0; i < 3; i++) {
			if(max[i] < a.min[i]) {
				ret = false;
				break;
			}
			if(min[i] > a.max[i]) {
				ret = false;
				break;
			}
		}
		Mark(ret);
		a.Mark(ret);
		return ret;
	}
	void Draw()
	{
		float xa = min[0];
		float ya = min[1];
		float za = min[2];
		float xb = max[0];
		float yb = max[1];
		float zb = max[2];
		if(mark) {
			glColor4f(1.0, 0.0, 0.0, 1.0);
		} else {
			glColor4f(0.1, 0.1, 0.5, 1.0);
		}
		glBegin( GL_LINES );
		glVertex3f(xa, ya, za);
		glVertex3f(xb, ya, za);
		glVertex3f(xa, yb, za);
		glVertex3f(xb, yb, za);
		glVertex3f(xa, ya, za);
		glVertex3f(xa, yb, za);
		glVertex3f(xb, ya, za);
		glVertex3f(xb, yb, za);
		
		glVertex3f(xa, ya, zb);
		glVertex3f(xb, ya, zb);
		glVertex3f(xa, yb, zb);
		glVertex3f(xb, yb, zb);
		glVertex3f(xa, ya, zb);
		glVertex3f(xa, yb, zb);
		glVertex3f(xb, ya, zb);
		glVertex3f(xb, yb, zb);
		
		glVertex3f(xa, ya, za);
		glVertex3f(xa, ya, zb);
		glVertex3f(xb, ya, za);
		glVertex3f(xb, ya, zb);
		glVertex3f(xa, yb, za);
		glVertex3f(xa, yb, zb);
		glVertex3f(xb, yb, za);
		glVertex3f(xb, yb, zb);
		
		glEnd();
	}
};

float frand() {
	return (float(rand()) / float(0x7FFF)) * 2.0 - 1.0;
}

#define WIN_X  1280
#define WIN_Y  720
#define PI     3.14159265358979
void swapBuffers() {
	wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
}

int main()
{
	WinApp win("TEST", WIN_X, WIN_Y);
	InitOpenGL(win.GetHwnd());

	int frame = 0;
	const int size = 10000;
	unsigned long startTime = timeGetTime();

	startTime = timeGetTime();
	
	printf("Generate Box");
	std::vector<Box> vBox(size);
	for(int i = 0; i < vBox.size(); i++) {
		float xo = frand() * 3.0;
		float yo = frand() * 3.0;
		float zo = frand() * 3.0;
		
		float w  = 0.05;
		float xa = -w;
		float ya = -w;
		float za = -w;
		float xb =  w;
		float yb =  w;
		float zb =  w;
		vBox[i] = Box(
			(xa < xb ? xa : xb) + xo,
			(ya < yb ? ya : yb) + yo,
			(za < zb ? za : zb) + zo,
			(xa > xb ? xa : xb) + xo,
			(ya > yb ? ya : yb) + yo,
			(za > zb ? za : zb) + zo);
			
	}
	printf("Done = %d[ms]\n", timeGetTime() - startTime);

	startTime = timeGetTime();
	Box *box = &vBox[0];
	printf("HitCheck Box");
	{
		const int size = vBox.size();
		for(int j = 0; j < size; j++) {
			for(int i = 0; i < size; i++) {
				if(i == j) continue;
				box[j].Hit(box[i]);
			}
		}
	}
	printf("Done = %d[ms]\n", timeGetTime() - startTime);
	float t = 0.0;
	while(win.ProcMsg())
	{
		glViewport(0, 0, win.GetWidth(), win.GetHeight());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, float(win.GetWidth()) / float(win.GetHeight()), 0.01, 1280.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		float radius = 10.0;
		gluLookAt(
			sin(t) * radius, cos(t) * radius, cos(t) * radius,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		for(int i = 0; i < size; i++) {
			vBox[i].Draw();
		}

		glFlush();
		swapBuffers();
		t += 0.005;
		if(t > 2 * PI) {
			t -= 2 * PI;
		}
	}
}

