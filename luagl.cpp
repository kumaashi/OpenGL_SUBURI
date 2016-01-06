#include "luagl.h"
#include "luamain.h"

namespace {
	//--------------------------------------------------------------------------------------
	// OpenGL Context
	//--------------------------------------------------------------------------------------
	HWND hWnd = NULL;
	HDC hDC = NULL;

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


	void glInitFunc()
	{
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
		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
		glActiveTexture(GL_TEXTURE2);
		glActiveTexture(GL_TEXTURE3);
	}


	//--------------------------------------------------------------------------------------
	// glSetInterval
	//--------------------------------------------------------------------------------------
	void glSetInterval(int isinterval)
	{
		BOOL (WINAPI *wglSwapIntervalEXT)(int) = NULL;
		const char *name = "WGL_EXT_swap_control";
		if( strstr( (char*)glGetString( GL_EXTENSIONS ), name) == 0)
		{
			printf("%s is not have ext\n", name);
			return;
		}
		//wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("WGL_EXT_swap_control");
		wglSwapIntervalEXT = (BOOL (WINAPI*)(int))wglGetProcAddress("wglGetSwapIntervalEXT");
		if(wglSwapIntervalEXT) wglSwapIntervalEXT(isinterval);
	}

	//--------------------------------------------------------------------------------------
	// luafunc
	//--------------------------------------------------------------------------------------
	int InitOpenGL(lua_State *l)
	{
		hWnd = luawindow::GetHwnd();
		if(hWnd == NULL) {
			printf("hWnd is NULL");
			LUARETURN_NIL(l, 0);
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
		LUARETURN(l, 1);
	}
	
	int TermOpenGL(lua_State *l)
	{
		if(hDC)
		{
			ReleaseDC(hWnd, hDC);
			hDC = NULL;
			hWnd = NULL;
		}
		LUARETURN(l, 1);
	}
}

namespace luagl
{
	void Init(lua_State *l)
	{
		LUAPUSHGFN(l, InitOpenGL);
		LUAPUSHGFN(l, TermOpenGL);
	}
}
