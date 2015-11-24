#include "include/util.h"
#include "include/file.h"

//--------------------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------------------
namespace {
	BOOL  Windowed      = TRUE;
	int   Width         = DEFAULT_WIDTH;
	int   Height        = DEFAULT_HEIGHT;
	static int xPos     = 0;
	static int yPos     = 0;
	static BOOL mLeft   = FALSE;
	static BOOL mRight  = FALSE;
	static BOOL mCenter = FALSE;

	static int a = 1, b = 234567, c = 890123;
}

int   GetWidth ()     { return Width; }
int   GetHeight()     { return Height; }
BOOL  GetMouseLeft()  { return mLeft; }
BOOL  GetMouseRight() { return mRight; }
POINT GetMousePos()   { POINT p = {xPos, yPos}; return p; }
int   random()        { a += b; b += c; c += a; return (a >> 16); }
float frand()         { return float(random()) / float(0x7FFF); }

//--------------------------------------------------------------------------------------
// show_fps
// fps : http://www.t-pot.com/program/13_fps/index.html
//--------------------------------------------------------------------------------------
void show_fps() {
	static DWORD    last = timeGetTime();
	static DWORD    frames = 0;
	static char     buf[256] = "";
	DWORD           current;
	current = timeGetTime();
	frames++;
	if(1000 <= current - last) {
		float dt = (float)(current - last) / 1000.0f;
		float fps = (float)frames / dt;
		last = current;
		frames = 0;
		sprintf(buf, "%.02f fps", fps);
		printf("%s\n", buf);
	}
}


//--------------------------------------------------------------------------------------
//
// Windows Function
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// ProcMsg
//--------------------------------------------------------------------------------------
BOOL ProcMsg() {
	MSG msg;
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT) return FALSE;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return TRUE;
}

//--------------------------------------------------------------------------------------
// Handler
//--------------------------------------------------------------------------------------
static void (*ProcHandleResize)(int, int); //Width, Height
void AddEvent_WM_SIZE(void (*proc)(int, int)) { ProcHandleResize = proc; }

//--------------------------------------------------------------------------------------
// WindowProc
//--------------------------------------------------------------------------------------
LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	int temp = wParam & 0xFFF0;
	switch(msg) {
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
		if(temp == SC_MONITORPOWER || temp == SC_SCREENSAVE) {
			return 0;
		}
		break;
	case WM_IME_SETCONTEXT:
		lParam &= ~ISC_SHOWUIALL;
		break;
	case WM_SIZE:
		Width  = LOWORD(lParam);
		Height = HIWORD(lParam);
		if(ProcHandleResize) {
			ProcHandleResize(Width, Height);
		}
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

int Init(int argc, char *argv[], void (*StartMain)(int argc, char *argv[], HDC hdc)) {
	//Create Windows
	const char *appname = argv[0];
	static WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
		WindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL, appname, NULL
	};
	wcex.hInstance = GetModuleHandle(NULL);
	DWORD style   = Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
	DWORD styleex = WS_EX_APPWINDOW | (Windowed ? WS_EX_WINDOWEDGE : 0);
	
	RECT   rc;
	SetRect(&rc, 0, 0, Width, Height);
	AdjustWindowRectEx(&rc, style, FALSE, styleex);
	rc.right  -= rc.left;
	rc.bottom -= rc.top;
	if (!RegisterClassEx(&wcex)) {
		printf("%s : failed RegisterClassEx\n", __FUNCTION__);
		return -1;
	}
	HWND hWnd = CreateWindowEx(
	           styleex, appname, appname, style,
	           (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2,
	           (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
	           rc.right, rc.bottom, 0, 0, wcex.hInstance, 0);
	if(!hWnd) {
		printf("%s : failed CreateWindowEx\n", __FUNCTION__);
		return -1;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	if(!Windowed) {
		static DEVMODEA dm = {
			{0}, 0, 0, sizeof(DEVMODEA), 0, DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
			{0}, 0, 0, 0, 0, 0, {0}, 0, 32,
			static_cast<DWORD>(Width),
			static_cast<DWORD>(Height)
		};
		ChangeDisplaySettingsA(&dm, CDS_FULLSCREEN);
	}
	
	//PreInitialize
	ShowCursor(Windowed ? TRUE : FALSE);
	const HDC hdc = GetDC(hWnd);
	static const PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 32
	};

	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
	wglMakeCurrent(hdc, wglCreateContext(hdc));
	glInitFunc();
	StartMain(argc, argv, hdc);
	ReleaseDC(hWnd, hdc);
	
	return 0;
}


//--------------------------------------------------------------------------------------
//
// OpenGL Function
//
//--------------------------------------------------------------------------------------
PFNGLCREATEPROGRAMPROC                  glCreateProgram             = NULL;
PFNGLDELETEPROGRAMPROC                  glDeleteProgram             = NULL;
PFNGLCREATESHADERPROC                   glCreateShader              = NULL;
PFNGLSHADERSOURCEPROC                   glShaderSource              = NULL;
PFNGLCOMPILESHADERPROC                  glCompileShader             = NULL;
PFNGLATTACHSHADERPROC                   glAttachShader              = NULL;
PFNGLDELETESHADERPROC                   glDeleteShader              = NULL;
PFNGLLINKPROGRAMPROC                    glLinkProgram               = NULL;
PFNGLUSEPROGRAMPROC                     glUseProgram                = NULL;
PFNGLUNIFORM1FPROC                      glUniform1f                 = NULL;
PFNGLUNIFORM1IPROC                      glUniform1i                 = NULL;
PFNGLUNIFORM4FVPROC                     glUniform4fv                = NULL;
PFNGLUNIFORMMATRIX4FVPROC               glUniformMatrix4fv          = NULL;
PFNGLGETUNIFORMLOCATIONPROC             glGetUniformLocation        = NULL;
PFNGLGENBUFFERSPROC                     glGenBuffers                = NULL;
PFNGLDELETEBUFFERSPROC                  glDeleteBuffers             = NULL;
PFNGLBINDBUFFERPROC                     glBindBuffer                = NULL;
PFNGLBINDBUFFERBASEPROC                 glBindBufferBase            = NULL;
PFNGLBUFFERDATAPROC                     glBufferData                = NULL;
PFNGLGETBUFFERSUBDATAPROC               glGetBufferSubData          = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC      glTransformFeedbackVaryings = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC         glBeginTransformFeedback    = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC           glEndTransformFeedback      = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC        glEnableVertexAttribArray   = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC       glDisableVertexAttribArray  = NULL;
PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer       = NULL;
PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog          = NULL;
PFNGLTEXIMAGE3DPROC                     glTexImage3D                = NULL;
PFNGLPROGRAMUNIFORM1IPROC               glProgramUniform1i          = NULL;
PFNGLACTIVETEXTUREPROC                  glActiveTexture             = NULL;
PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog         = NULL;
PFNGLGETPROGRAMIVPROC                   glGetProgramiv              = NULL;
PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation         = NULL;
PFNGLBINDVERTEXARRAYPROC                glBindVertexArray           = NULL;
PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation        = NULL;
PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays           = NULL;
PFNGLDELETEVERTEXARRAYSPROC             glDeleteVertexArrays        = NULL;
PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers           = NULL;
PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer           = NULL;
PFNGLGENRENDERBUFFERSPROC               glGenRenderbuffers          = NULL;
PFNGLBINDRENDERBUFFERPROC               glBindRenderBuffer          = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC        glFramebufferRenderbuffer   = NULL;
PFNGLRENDERBUFFERSTORAGEPROC            glRenderBufferStorage       = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC             glFramebufferTexture        = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC           glFramebufferTexture2D      = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC         glCheckFramebufferStatus    = NULL;
PFNGLDRAWBUFFERSPROC                    glDrawBuffers               = NULL;
PFNGLGENERATEMIPMAPPROC                 glGenerateMipmap            = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC          glTexImage2DMultisample     = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample = NULL;
PFNGLRENDERBUFFERSTORAGEPROC            glRenderbufferStorage       = NULL;
PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer           = NULL;
PFNGLGETSHADERIVPROC                    glGetShaderiv               = NULL;
PFNGLDELETEFRAMEBUFFERSPROC             glDeleteFramebuffers        = NULL;
PFNGLDELETERENDERBUFFERSPROC            glDeleteRenderbuffers       = NULL;

static void glErrorCallbackUser (GLenum source, GLenum type, GLuint id,
		   GLenum severity, GLsizei length,  const GLchar* message, const void* userParam)
{
	//std::vector<char> vmsg;
	//vmsg.resize(length * 2); // tekitouwwwww
	//memset(&vmsg[0], 0, vmsg.size());
	printf("glErrorCallbackUser  %s\n", message);
}

void glInitFunc() {
	glCreateProgram             = (PFNGLCREATEPROGRAMPROC             )wglGetProcAddress("glCreateProgram");
	glDeleteProgram             = (PFNGLDELETEPROGRAMPROC             )wglGetProcAddress("glDeleteProgram");
	glCreateShader              = (PFNGLCREATESHADERPROC              )wglGetProcAddress("glCreateShader");
	glShaderSource              = (PFNGLSHADERSOURCEPROC              )wglGetProcAddress("glShaderSource");
	glCompileShader             = (PFNGLCOMPILESHADERPROC             )wglGetProcAddress("glCompileShader");
	glAttachShader              = (PFNGLATTACHSHADERPROC              )wglGetProcAddress("glAttachShader");
	glDeleteShader              = (PFNGLDELETESHADERPROC              )wglGetProcAddress("glDeleteShader");
	glLinkProgram               = (PFNGLLINKPROGRAMPROC               )wglGetProcAddress("glLinkProgram");
	glUseProgram                = (PFNGLUSEPROGRAMPROC                )wglGetProcAddress("glUseProgram");
	glUniform1f                 = (PFNGLUNIFORM1FPROC                 )wglGetProcAddress("glUniform1f");
	glUniform1i                 = (PFNGLUNIFORM1IPROC                 )wglGetProcAddress("glUniform1i");
	glUniform4fv                = (PFNGLUNIFORM4FVPROC                )wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC          )wglGetProcAddress("glUniformMatrix4fv");
	glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC        )wglGetProcAddress("glGetUniformLocation");
	glGenBuffers                = (PFNGLGENBUFFERSPROC                )wglGetProcAddress("glGenBuffers");
	glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC             )wglGetProcAddress("glDeleteBuffers");
	glBindBuffer                = (PFNGLBINDBUFFERPROC                )wglGetProcAddress("glBindBuffer");
	glBindBufferBase            = (PFNGLBINDBUFFERBASEPROC            )wglGetProcAddress("glBindBufferBase");
	glBufferData                = (PFNGLBUFFERDATAPROC                )wglGetProcAddress("glBufferData");
	glGetBufferSubData          = (PFNGLGETBUFFERSUBDATAPROC          )wglGetProcAddress("glGetBufferSubData");
	glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC )wglGetProcAddress("glTransformFeedbackVaryings");
	glBeginTransformFeedback    = (PFNGLBEGINTRANSFORMFEEDBACKPROC    )wglGetProcAddress("glBeginTransformFeedback");
	glEndTransformFeedback      = (PFNGLENDTRANSFORMFEEDBACKPROC      )wglGetProcAddress("glEndTransformFeedback");
	glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC   )wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray  = (PFNGLDISABLEVERTEXATTRIBARRAYPROC  )wglGetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC       )wglGetProcAddress("glVertexAttribPointer");
	glGetShaderiv               = (PFNGLGETSHADERIVPROC               )wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC          )wglGetProcAddress("glGetShaderInfoLog");
	glTexImage3D                = (PFNGLTEXIMAGE3DPROC                )wglGetProcAddress("glTexImage3D");
	glProgramUniform1i          = (PFNGLPROGRAMUNIFORM1IPROC          )wglGetProcAddress("glProgramUniform1i");
	glActiveTexture             = (PFNGLACTIVETEXTUREPROC             )wglGetProcAddress("glActiveTexture");
	glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC         )wglGetProcAddress("glGetProgramInfoLog");
	glGetProgramiv              = (PFNGLGETPROGRAMIVPROC              )wglGetProcAddress("glGetProgramiv");
	glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC         )wglGetProcAddress("glGetAttribLocation");
	glBindVertexArray           = (PFNGLBINDVERTEXARRAYPROC           )wglGetProcAddress("glBindVertexArray");
	glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC        )wglGetProcAddress("glBindAttribLocation");
	glGenVertexArrays           = (PFNGLGENVERTEXARRAYSPROC           )wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays        = (PFNGLDELETEVERTEXARRAYSPROC        )wglGetProcAddress("glDeleteVertexArrays");
	glGenFramebuffers           = (PFNGLGENFRAMEBUFFERSPROC           )wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer           = (PFNGLBINDFRAMEBUFFERPROC           )wglGetProcAddress("glBindFramebuffer");
	glGenRenderbuffers          = (PFNGLGENRENDERBUFFERSPROC          )wglGetProcAddress("glGenRenderbuffers");
	glBindRenderBuffer          = (PFNGLBINDRENDERBUFFERPROC          )wglGetProcAddress("glBindRenderbuffer");
	glFramebufferRenderbuffer   = (PFNGLFRAMEBUFFERRENDERBUFFERPROC   )wglGetProcAddress("glFramebufferRenderbuffer");
	glRenderBufferStorage       = (PFNGLRENDERBUFFERSTORAGEPROC       )wglGetProcAddress("glRenderbufferStorage");
	glFramebufferTexture        = (PFNGLFRAMEBUFFERTEXTUREPROC        )wglGetProcAddress("glFramebufferTexture");
	glFramebufferTexture2D      = (PFNGLFRAMEBUFFERTEXTURE2DPROC      )wglGetProcAddress("glFramebufferTexture2D");
	glCheckFramebufferStatus    = (PFNGLCHECKFRAMEBUFFERSTATUSPROC    )wglGetProcAddress("glCheckFramebufferStatus");
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC               )wglGetProcAddress("glDrawBuffers");
	glDrawBuffers               = (PFNGLDRAWBUFFERSPROC               )wglGetProcAddress("glDrawBuffers");
	glGenerateMipmap            = (PFNGLGENERATEMIPMAPPROC            )wglGetProcAddress("glGenerateMipmap");
	glTexImage2DMultisample     = (PFNGLTEXIMAGE2DMULTISAMPLEPROC     )wglGetProcAddress("glTexImage2DMultisample");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	glRenderbufferStorage       = (PFNGLRENDERBUFFERSTORAGEPROC       )wglGetProcAddress("glRenderbufferStorage");
	glBlitFramebuffer           = (PFNGLBLITFRAMEBUFFERPROC           )wglGetProcAddress("glBlitFramebuffer");
	glDeleteFramebuffers        = (PFNGLDELETEFRAMEBUFFERSPROC        )wglGetProcAddress("glDeleteFramebuffers");
	glDeleteRenderbuffers       = (PFNGLDELETERENDERBUFFERSPROC       )wglGetProcAddress("glDeleteRenderbuffers");
	
	
	
	//glEnable( GL_MULTISAMPLE );
	/*
	glDebugMessageCallback((GLDEBUGPROC)glErrorCallbackUser, NULL);
	*/
	
}

//--------------------------------------------------------------------------------------
// glSetInterval
//--------------------------------------------------------------------------------------
void glSetInterval(int isinterval) {
	BOOL (WINAPI *wglSwapIntervalEXT)(int) = NULL;
	const char *name = "WGL_EXT_swap_control";
	if( strstr( (char*)glGetString( GL_EXTENSIONS ), name) == 0) {
		printf("%s is not have ext\n", name);
		return;
	}
	//wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("WGL_EXT_swap_control");
	wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("wglGetSwapIntervalEXT");
	if(wglSwapIntervalEXT) wglSwapIntervalEXT(isinterval);
}

