#include "util.h"

//namepsace Util?????

//--------------------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------------------
BOOL  Windowed      = TRUE;
int   Width         = DEFAULT_WIDTH;
int   Height        = DEFAULT_HEIGHT;
std::vector<char> diag;


//--------------------------------------------------------------------------------------
// OpenGL Function
//--------------------------------------------------------------------------------------
PFNGLCREATEPROGRAMPROC                  glCreateProgram             = NULL;
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
PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer       = NULL;
PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog          = NULL;
PFNGLTEXIMAGE3DPROC                     glTexImage3D                = NULL;
PFNGLPROGRAMUNIFORM1IPROC               glProgramUniform1i          = NULL;
PFNGLACTIVETEXTUREPROC                  glActiveTexture             = NULL;
PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog         = NULL;
PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation         = NULL;
PFNGLBINDVERTEXARRAYPROC                glBindVertexArray           = NULL;
PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation        = NULL;
PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays           = NULL;
PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers           = NULL;
PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer           = NULL;
PFNGLGENRENDERBUFFERSPROC               glGenRenderBuffers          = NULL;
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
PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer           = NULL;

void glInitFunc() {
	glCreateProgram             = (PFNGLCREATEPROGRAMPROC             )wglGetProcAddress("glCreateProgram");
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
	glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC       )wglGetProcAddress("glVertexAttribPointer");
	glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC          )wglGetProcAddress("glGetShaderInfoLog");
	glTexImage3D                = (PFNGLTEXIMAGE3DPROC                )wglGetProcAddress("glTexImage3D");
	glProgramUniform1i          = (PFNGLPROGRAMUNIFORM1IPROC          )wglGetProcAddress("glProgramUniform1i");
	glActiveTexture             = (PFNGLACTIVETEXTUREPROC             )wglGetProcAddress("glActiveTexture");
	glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC         )wglGetProcAddress("glGetProgramInfoLog");
	glGetAttribLocation         = (PFNGLGETATTRIBLOCATIONPROC         )wglGetProcAddress("glGetAttribLocation");
	glBindVertexArray           = (PFNGLBINDVERTEXARRAYPROC           )wglGetProcAddress("glBindVertexArray");
	glBindAttribLocation        = (PFNGLBINDATTRIBLOCATIONPROC        )wglGetProcAddress("glBindAttribLocation");
	glGenVertexArrays           = (PFNGLGENVERTEXARRAYSPROC           )wglGetProcAddress("glGenVertexArrays");
	glGenFramebuffers           = (PFNGLGENFRAMEBUFFERSPROC           )wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer           = (PFNGLBINDFRAMEBUFFERPROC           )wglGetProcAddress("glBindFramebuffer");
	glGenRenderBuffers          = (PFNGLGENRENDERBUFFERSPROC          )wglGetProcAddress("glGenRenderbuffers");
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
	glBlitFramebuffer           = (PFNGLBLITFRAMEBUFFERPROC           )wglGetProcAddress("glBlitFramebuffer");
	
	
	//check
	printf("glCreateProgram             =%08X\n", glCreateProgram             );
	printf("glCreateShader              =%08X\n", glCreateShader              );
	printf("glShaderSource              =%08X\n", glShaderSource              );
	printf("glCompileShader             =%08X\n", glCompileShader             );
	printf("glAttachShader              =%08X\n", glAttachShader              );
	printf("glDeleteShader              =%08X\n", glDeleteShader              );
	printf("glLinkProgram               =%08X\n", glLinkProgram               );
	printf("glUseProgram                =%08X\n", glUseProgram                );
	printf("glUniform1f                 =%08X\n", glUniform1f                 );
	printf("glUniform1i                 =%08X\n", glUniform1i                 );
	printf("glUniform4fv                =%08X\n", glUniform4fv                );
	printf("glUniformMatrix4fv          =%08X\n", glUniformMatrix4fv          );
	printf("glGetUniformLocation        =%08X\n", glGetUniformLocation        );
	printf("glGenBuffers                =%08X\n", glGenBuffers                );
	printf("glDeleteBuffers             =%08X\n", glDeleteBuffers             );
	printf("glBindBuffer                =%08X\n", glBindBuffer                );
	printf("glBindBufferBase            =%08X\n", glBindBufferBase            );
	printf("glBufferData                =%08X\n", glBufferData                );
	printf("glGetBufferSubData          =%08X\n", glGetBufferSubData          );
	printf("glTransformFeedbackVaryings =%08X\n", glTransformFeedbackVaryings );
	printf("glBeginTransformFeedback    =%08X\n", glBeginTransformFeedback    );
	printf("glEndTransformFeedback      =%08X\n", glEndTransformFeedback      );
	printf("glEnableVertexAttribArray   =%08X\n", glEnableVertexAttribArray   );
	printf("glVertexAttribPointer       =%08X\n", glVertexAttribPointer       );
	printf("glGetShaderInfoLog          =%08X\n", glGetShaderInfoLog          );
	printf("glTexImage3D                =%08X\n", glTexImage3D                );
	printf("glProgramUniform1i          =%08X\n", glProgramUniform1i          );
	printf("glActiveTexture             =%08X\n", glActiveTexture             );
	printf("glGetProgramInfoLog         =%08X\n", glGetProgramInfoLog         );
	printf("glGetAttribLocation         =%08X\n", glGetAttribLocation         );
	printf("glBindVertexArray           =%08X\n", glBindVertexArray           );
	printf("glBindAttribLocation        =%08X\n", glBindAttribLocation        );
	printf("glGenVertexArrays           =%08X\n", glGenVertexArrays           );
	printf("glGenFramebuffers           =%08X\n", glGenFramebuffers           );
	printf("glBindFramebuffer           =%08X\n", glBindFramebuffer           );
	printf("glGenRenderBuffers          =%08X\n", glGenRenderBuffers          );
	printf("glBindRenderBuffer          =%08X\n", glBindRenderBuffer          );
	printf("glFramebufferRenderbuffer   =%08X\n", glFramebufferRenderbuffer   );
	printf("glRenderBufferStorage       =%08X\n", glRenderBufferStorage       );
	printf("glFramebufferTexture        =%08X\n", glFramebufferTexture        );
	printf("glFramebufferTexture2D      =%08X\n", glFramebufferTexture2D      );
	printf("glCheckFramebufferStatus    =%08X\n", glCheckFramebufferStatus    );
	printf("glTexImage2DMultisample     =%08X\n", glTexImage2DMultisample     );
	printf("glRenderbufferStorageMultisample    =%08X\n", glRenderbufferStorageMultisample);
	
}


int random() {
	static int a = 1, b = 234567, c = 890123;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

float frand() {
	return float(random()) / float(0x7FFF);
}


//--------------------------------------------------------------------------------------
// OpenGL Ex
//--------------------------------------------------------------------------------------
void glSetInterval(int isinterval) {
	BOOL (WINAPI *wglSwapIntervalEXT)(int) = NULL;
	if( strstr( (char*)glGetString( GL_EXTENSIONS ), "WGL_EXT_swap_control") == 0) return;
	wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("WGL_EXT_swap_control");
	if(wglSwapIntervalEXT) wglSwapIntervalEXT(isinterval);
}

//--------------------------------------------------------------------------------------
// glProgram Loader
//--------------------------------------------------------------------------------------
GLuint glLoadShader(const char *vsfile, const char *fsfile) {
	File fp;
	char *b = NULL;
	char *s = NULL;

	GLsizei size;

	//VS
	printf("Load %s %s:", vsfile, fsfile);
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	fp.Open(vsfile, "r");
	b = fp.Buf();
	glShaderSource(vs, 1, &b, NULL);
	glCompileShader(vs);
	memset(&diag[0], 0, diag.size());
	glGetShaderInfoLog(vs, diag.size(), &size, &diag[0]);
	printf("%s:", vsfile);
	s = &diag[0];
	while(*s) printf("%c", *s++);
	
	//FS
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	fp.Open(fsfile, "r");
	b = fp.Buf();
	glShaderSource(fs, 1, &b, NULL);
	glCompileShader(fs);
	memset(&diag[0], 0, diag.size());
	glGetShaderInfoLog(fs, diag.size(), &size, &diag[0]);
	printf("%s:", fsfile);
	s = &diag[0];
	while(*s) printf("%c", *s++);
	printf("%s %s Done\n", vsfile, fsfile);
	
	GLuint ret = glCreateProgram();
	glAttachShader(ret, vs);
	glAttachShader(ret, fs);
	glDeleteShader(fs);
	glDeleteShader(vs);
	glLinkProgram(ret);
	
	return ret;
}


//--------------------------------------------------------------------------------------
// Windows
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

LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	int temp = wParam & 0xFFF0;
	switch(msg) {
	case WM_SYSCOMMAND:
		if(temp == SC_MONITORPOWER || temp == SC_SCREENSAVE) {
			return 0;
		}
		break;
	case WM_IME_SETCONTEXT:
		lParam &= ~ISC_SHOWUIALL;
		break;
	case WM_SIZE:
		Width	= LOWORD(lParam);
		Height = HIWORD(lParam);
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int Init(int argc, char *argv[], void (*StartMain)(int argc, char *argv[], HDC hdc)) {
	diag.resize(32768);
	
	//Create Windows
	const char *appname = argv[0];
	static WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
		WindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL, appname, NULL
	};
	wcex.hInstance = GetModuleHandle(NULL);
	RegisterClassEx(&wcex);
	
	//Create Windows
	DWORD styleex = WS_EX_APPWINDOW | (Windowed ? WS_EX_WINDOWEDGE : 0);
	DWORD style	 = Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
	HWND hWnd		 = CreateWindowEx(
			styleex, appname, appname, style, 0, 0, Width * 1.5, Height * 1.5, NULL, NULL, wcex.hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	if(!Windowed) {
		static DEVMODEA dm = {
			{0}, 0, 0, sizeof(DEVMODEA), 0, DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
			{0}, 0, 0, 0, 0, 0, {0}, 0, 32, Width, Height
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
	
	return 0;
}

