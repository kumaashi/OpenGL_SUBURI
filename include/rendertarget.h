#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "util.h"

//------------------------------------------------------------------------------------------
// RenderTarget
//------------------------------------------------------------------------------------------

//#define _USE_MS_

class RenderTarget {
	enum {
		Max = 4,
	};
	GLuint fbo;
	GLuint rbo[Max];
	GLuint rbdepth;
	GLuint rttex[Max];
	int Width;
	int Height;
	int Sample;
	std::string Name;

	void glSetupState() {
		struct Param {
			GLenum pname;
			GLint  param;
		};
		Param param[] = {
			//{ GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST},
			{ GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST},
			{ GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE},
			{ GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE},
			{ GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE},
			{ GL_GENERATE_MIPMAP,    GL_FALSE},
			{ 0, 0},
		};
		
		for(int i = 0 ; param[i].pname; i++) {
			Param *w = &param[i];
			glTexParameteri(GL_TEXTURE_2D, w->pname, w->param); GL_DEBUG0;
		}
	}

	void SetupDrawBuffers() {
		GLuint attachments[Max] = {
			GL_COLOR_ATTACHMENT0 + 0,
			GL_COLOR_ATTACHMENT0 + 1,
			GL_COLOR_ATTACHMENT0 + 2,
			GL_COLOR_ATTACHMENT0 + 3,
		}; 
		glDrawBuffers(Max, attachments ); GL_DEBUG0;
	}

public:
	int  GetWidth() { return Width; }
	int  GetHeight() { return Width; }
	RenderTarget()  {
		fbo = 0;
		/*
		if(!fbo < 0) {
			glGenFramebuffers(1, &fbo);
		}
		glGenRenderBuffers(1, &rbo);
		GL_DEBUG0;
		glGenTextures(Max, rttex);
		GL_DEBUG0;
		*/
	}
	~RenderTarget() {
		Release();
	}

	void Release() {
		if(fbo) {
			glDeleteFramebuffers(1, &fbo); GL_DEBUG0;
			glDeleteRenderBuffers(Max, rbo); GL_DEBUG0;
			glDeleteRenderBuffers(1, &rbdepth); GL_DEBUG0;
			glDeleteTextures(Max, rttex); GL_DEBUG0;
			memset(rttex, 0, sizeof(rttex));
			memset(rbo, 0, sizeof(rbo));
			
			rbdepth = 0;
			fbo = 0;
			
		}
	}
	
	void Create(const char *name, int w, int h, int ms = 8) {
		Name = std::string(name);
		
		printf("%s: %s, Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, name, w, h, ms);
		int status = 0;
		GL_DEBUG0;

		//Create Render Buffer and texture
		glGenFramebuffers(1, &fbo); GL_DEBUG0;
		glGenRenderBuffers(Max, rbo); GL_DEBUG0;
		glGenRenderBuffers(1, &rbdepth); GL_DEBUG0;
		glGenTextures(Max, rttex); GL_DEBUG0;

		//Bind
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); GL_DEBUG0;

		//Color
		for(int i = 0 ; i < Max; i++) {
			glBindRenderBuffer(GL_RENDERBUFFER, rbo[i]); GL_DEBUG0;
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_RGBA32F, w, h); GL_DEBUG0;
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rbo[i]); GL_DEBUG0;
		}

		//Depth
		glBindRenderBuffer(GL_RENDERBUFFER, rbdepth); GL_DEBUG0;
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h); GL_DEBUG0;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbdepth); GL_DEBUG0;

		for(int i = 0 ; i < Max; i++) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex[i]); GL_DEBUG0;
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE); GL_DEBUG0;
			glSetupState();
			glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, rttex[i], 0); GL_DEBUG0;
		}
		SetupDrawBuffers();
		
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			printf("Bind Failed : status=%08X\n", status);
			Sleep(5000);
		} else {
			for(int i = 0; i < Max; i++) {
				printf("Bind OK index=%d: %d, %d\n", i, rttex[i], fbo);
			}
		}
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0); GL_DEBUG0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_DEBUG0;
		glBindRenderBuffer(GL_RENDERBUFFER, 0); GL_DEBUG0;
		Width  = w;
		Height = h;
		Sample = ms;
	}
	
	void Bind() {
		glEnable(GL_TEXTURE_2D); GL_DEBUG0;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		for(int i = 0 ; i < Max; i++) {
			glActiveTexture(GL_TEXTURE0 + i); GL_DEBUG0;
			glBindTexture(GL_TEXTURE_2D, rttex[i]); GL_DEBUG0;
			printf("%s : fbo:%d bindtexture -> %d\n", __FUNCTION__, fbo, rttex[i]);
		}
		SetupDrawBuffers();
		
		//glActiveTexture(GL_TEXTURE0); GL_DEBUG0;
	}

	void Unbind() {
		GL_DEBUG0;
		for(int i = 0 ; i < Max; i++) {
			glActiveTexture(GL_TEXTURE0 + i); GL_DEBUG0;
			glBindTexture(GL_TEXTURE_2D, 0); GL_DEBUG0;
		}
		glActiveTexture(GL_TEXTURE0); GL_DEBUG0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Begin() {
		GL_DEBUG0;
		GLint oldfbo = -1;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldfbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); GL_DEBUG0;
		SetupDrawBuffers(); GL_DEBUG0;
		printf("RT : Old = %08X, New=%08X\n", oldfbo, fbo);
	}


	void Resolve() {
	}

	void End() {
		GL_DEBUG0;
		Resolve();
		
		//Create MipMap
		for(int i = 0 ; i < Max; i++) {
			glActiveTexture(GL_TEXTURE0 + i); GL_DEBUG0;
			glGenerateMipmap(GL_TEXTURE_2D); GL_DEBUG0;
		}
		glActiveTexture(GL_TEXTURE0); GL_DEBUG0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_DEBUG0;

		GLint oldfbo = -1;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldfbo);
		printf("END fbo = %08X\n", oldfbo);
		printf("---------------------------------------------");
	}
};


#endif

