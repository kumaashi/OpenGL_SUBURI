#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "util.h"

//------------------------------------------------------------------------------------------
// RenderTarget
//------------------------------------------------------------------------------------------

//#define _USE_MS_


struct RenderTarget {
	enum {
		Max = 4,
	};
	GLuint fbo;
	GLuint rbo;
	GLuint rttex[Max];
	int Width, Height, Sample;

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
			glTexParameteri(GL_TEXTURE_2D, w->pname, w->param);
		}
	}
	
	void SetupDrawBuffers() {
		GLuint attachments[Max] = {
			GL_COLOR_ATTACHMENT0 + 0,
			GL_COLOR_ATTACHMENT0 + 1,
			GL_COLOR_ATTACHMENT0 + 2,
			GL_COLOR_ATTACHMENT0 + 3,
		}; 
		glDrawBuffers(Max, attachments );
	}
	
	void Create(int w, int h, int ms = 8) {
		printf("%s: Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, w, h, ms);
		int status = 0;
		glGenFramebuffers(1, &fbo);
		glGenRenderBuffers(1, &rbo);
		glGenTextures(Max, rttex);

		glBindRenderBuffer(GL_RENDERBUFFER, rbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

#ifdef _USE_MS_
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h);
#else //_USE_MS_
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, w, h);
#endif //_USE_MS_


		glEnable(GL_TEXTURE_2D);
		for(int i = 0 ; i < Max; i++) {
#ifdef _USE_MS_
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex[i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
			glSetupState();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, rttex[i], 0);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
#else //_USE_MS_
			glBindTexture(GL_TEXTURE_2D, rttex[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
			glSetupState();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, rttex[i], 0);
			glBindTexture(GL_TEXTURE_2D, 0);
#endif //_USE_MS_
			//glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		glDisable(GL_TEXTURE_2D);

		SetupDrawBuffers();
		
		if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
			Sleep(5000);
		} else {
			for(int i = 0; i < Max; i++) {
				printf("Bind OK index=%d: %d, %d\n", i, rttex[i], fbo);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderBuffer(GL_RENDERBUFFER, 0);
		Width  = w;
		Height = h;
		Sample = ms;
	}

	void Begin() {
		glEnable(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		for(int i = 0 ; i < Max; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, rttex[i]);
		}
		SetupDrawBuffers();
	}

	void End() {
		//Resolve();
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};


#endif
