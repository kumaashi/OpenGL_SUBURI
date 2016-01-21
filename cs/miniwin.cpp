///////////////////////////////////////////////////////////////////////////////////////////////
//
// https://www.opengl.org/wiki/Compute_Shader
// http://wlog.flatlib.jp/item/1637
// 
// in uvec3 gl_NumWorkGroups;
// in uvec3 gl_WorkGroupID;
// in uvec3 gl_LocalInvocationID;
// in uvec3 gl_GlobalInvocationID; //gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID;
// in uint  gl_LocalInvocationIndex;
//
///////////////////////////////////////////////////////////////////////////////////////////////

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

#include "file.h"

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




void checkErrors(std::string desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		fprintf(stderr, "OpenGL error in \"%s\": %s (%d)\n", desc.c_str(), gluErrorString(e), e);
		//exit(20);
	}
}

int GetStatus(int type, GLuint ctx) {
	int index = -1;
	int rvalue = GL_TRUE;
	void (*fnStatus[])(GLuint, GLenum, GLint *) =
	{
		glGetShaderiv,
		glGetProgramiv,
	};
	void (*fnInfoLog[])(GLuint, GLsizei, GLsizei *, GLchar *) =
	{
		glGetShaderInfoLog,
		glGetProgramInfoLog,
	};
	if(type == GL_COMPILE_STATUS) index = 0;
	if(type == GL_LINK_STATUS)    index = 1;
	if(index < 0) {
		fprintf(stderr, "Invalid status type\n");
		return -1;
	}
	fnStatus[index](ctx, type, &rvalue);
	if (rvalue != GL_TRUE) {
		GLchar  log[10240];
		GLsizei length = 0;
		fnInfoLog[index](ctx, sizeof(log) - 1, &length, log);
		printf("Error fnindex=%d, length = %d, rvalue=%d\n", index, length, rvalue);
		fprintf(stderr, "[Error log]\n%s\n", log);
		return -1;
	}
	return 0;
}

GLuint LoadShader(GLenum shaderType, const char *name) {
	if(!name) return 0;
	GLuint shader = glCreateShader(shaderType);
	File file(name);
	const GLint len   = file.Size();
	const char *Src = static_cast<const char *>(file.Buf());
	glShaderSource(shader, 1, &Src, &len);
	glCompileShader(shader);
	if(GetStatus(GL_COMPILE_STATUS, shader)) {
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint CreateProgram(
	const GLenum *  shadertypes,
	const char   ** filenames,
	const char   ** attrname = NULL,
	const char   ** dataname = NULL)
{
	if(!shadertypes || !shadertypes) return 0;

	//GL_VERTEX_SHADER
	//GL_GEOMETRY_SHADER
	//GL_FRAGMENT_SHADER
	//GL_COMPUTE_SHADER

	GLuint progHandle = glCreateProgram();
	for(int i = 0; filenames[i] && shadertypes[i] ; i++) {
		GLuint shader = LoadShader(shadertypes[i], filenames[i]);
		if(!shader) continue;
		fprintf(stderr, "filename=%s, type=%08X shader=%08X\n", filenames[i], shadertypes[i], shader);
		glAttachShader(progHandle, shader);
		glDeleteShader(shader);
	}
	
	if(attrname) {
		for(int i = 0; attrname[i]; i++) {
			fprintf(stderr, "glBindAttribLocation -> %d %s\n", i, attrname[i]);
			glBindAttribLocation(progHandle, i, attrname[i]);
		}
	}

	if(dataname) {
		for(int i = 0; dataname[i]; i++) {
			fprintf(stderr, "glBindFragDataLocation -> %d %s\n", i, dataname[i]);
			glBindFragDataLocation(progHandle, i, dataname[i]);
		}
	}
	
	glLinkProgram(progHandle);
	if(GetStatus(GL_LINK_STATUS, progHandle)) {
		glDeleteProgram(progHandle);
		return 0;
	}
	return progHandle;
}

GLuint genComputeProg(GLuint texHandle) {
	const GLenum shadertypes[] = {
		GL_COMPUTE_SHADER,
		0,
	};

	const char *filenames[] = {
		"cs.glsl",
		0,
	};

	GLuint progHandle = CreateProgram(shadertypes, filenames, NULL, NULL);
	glUseProgram(progHandle);
	glUniform1i(glGetUniformLocation(progHandle, "destTex"), 0);
	glUniform1i(glGetUniformLocation(progHandle, "infoTex"), 1);
	checkErrors("Compute shader");
	return progHandle;
}


GLuint genRenderProg(GLuint texHandle) {
	const GLenum shadertypes[] = {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		0,
	};
	const char *filenames[] = {
		"vs.glsl",
		"fs.glsl",
		0,
	};
	const char *attrname[] = { NULL, };
	const char *dataname[] = { "color", NULL, };
	
	GLuint progHandle = CreateProgram(shadertypes, filenames, attrname, dataname);

	glUseProgram(progHandle);
	glUniform1i(glGetUniformLocation(progHandle, "srcTex"), 0);

	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);

	GLuint posBuf;
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	float data[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, data, GL_STREAM_DRAW);
	GLint posPtr = glGetAttribLocation(progHandle, "pos");
	glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posPtr);

	checkErrors("Render shaders");
	return progHandle;
}

GLuint genTexture(int width, int height) {
	std::vector<float> vBuf(width * height * sizeof(float) * 4);
	memset(&vBuf[0], 0, vBuf.size());
	for(int i = 0; i < vBuf.size(); i++) {
		vBuf[i] = float(i);
	}
	GLuint texHandle;

	glGenTextures(1, &texHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, &vBuf[0]);
	glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	checkErrors("Gen texture");	
	return texHandle;
}

GLuint genIntTexture(int width, int height) {
	std::vector<int> vBuf(width * height * sizeof(unsigned long) * 4);
	for(int i = 0; i < vBuf.size(); i++) {
		vBuf[i] = i;
	}

	GLuint texHandle;
	glGenTextures(1, &texHandle);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_INT, &vBuf[0]);
	glBindImageTexture(1, texHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32I);
	checkErrors("Gen texture");	
	return texHandle;
}




void swapBuffers() {
	wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
	checkErrors("Swapping bufs");
}


#define CELL_SIZE 32
#define WIN_X     (CELL_SIZE * 20)
#define WIN_Y     (CELL_SIZE * 15)

void updateTex(GLuint computeHandle, int frame) {
	glUseProgram(computeHandle);
	glUniform1f(glGetUniformLocation(computeHandle, "roll"), (float)frame*0.01f);

	int dispatchX = WIN_X / CELL_SIZE;
	int dispatchY = WIN_Y / CELL_SIZE;
	int dispatchZ = 1;

	//printf("%d %d %d\n", dispatchX, dispatchY, dispatchZ);
	glDispatchCompute(dispatchX, dispatchY, dispatchZ); // 512^2 threads in blocks of 16^2
	checkErrors("Dispatch compute shader");
}

void draw(GLuint renderHandle) {
	glUseProgram(renderHandle);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	swapBuffers();
	checkErrors("Draw screen");
}

int main()
{
	WinApp win("TEST", WIN_X, WIN_Y);
	InitOpenGL(win.GetHwnd());
	glViewport(0, 0, win.GetWidth(), win.GetHeight());

	GLuint texHandle     = genTexture( win.GetWidth(), win.GetHeight());
	GLuint texInt        = genIntTexture( win.GetWidth(), win.GetHeight());
	GLuint renderHandle  = genRenderProg(texHandle);
	GLuint computeHandle = genComputeProg(texHandle);

	std::vector<int> vBuf(WIN_X * WIN_Y * sizeof(unsigned long) * 4);
	memset(&vBuf[0], 0, vBuf.size());
	int frame = 0;
	while(win.ProcMsg())
	{
		if(GetAsyncKeyState(VK_F5) & 0x8000)
		{
			texHandle     = genTexture( win.GetWidth(), win.GetHeight());
			texInt        = genIntTexture( win.GetWidth(), win.GetHeight());
			renderHandle  = genRenderProg(texHandle);
			computeHandle = genComputeProg(texHandle);
		}
		
		updateTex(computeHandle, frame);
		draw(renderHandle);
		frame++;


		printf("texInt = %d\n", texInt);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texInt);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA_INTEGER, GL_INT, &vBuf[0]);

		checkErrors("Check glGetTexImage");
	}
}

