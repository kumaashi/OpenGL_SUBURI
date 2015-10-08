#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "util.h"

//------------------------------------------------------------------------------------------
// RenderTarget
//------------------------------------------------------------------------------------------
struct RenderTarget {
	enum {
		Max = 4,
	};
	GLuint fbo;
	GLuint fbo2;
	GLuint rttex[Max];
	GLuint rttex2;
	GLuint rbo;
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

	void Create(int w, int h, int ms = 8) {
		printf("%s: Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, w, h, ms);
		int status = 0;
		glGenFramebuffers(1, &fbo);
		glGenRenderBuffers(1, &rbo);
		glGenTextures(Max, rttex);  //1ST Create Texture

		glBindRenderBuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GLuint attachments[Max] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
		}; 

		glEnable(GL_TEXTURE_2D);
		for(int i = 0 ; i < Max; i++) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex[i]);
			glSetupState();
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D_MULTISAMPLE, rttex[i], 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		glDisable(GL_TEXTURE_2D);

		//check
		glDrawBuffers(Max, attachments );
		if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
			Sleep(5000);
		} else {
			for(int i = 0; i < Max; i++) {
				printf("Bind OK index=%d: %d, %d\n", i, rttex[i], fbo);
			}
		}
		glBindRenderBuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		printf("%s, DONE\n", __FUNCTION__);
		Width  = w;
		Height = h;
		Sample = ms;
	}

	void SetTexture() {
		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
		glActiveTexture(GL_TEXTURE2);
		glActiveTexture(GL_TEXTURE3);
		for(int i = 0 ; i < Max; i++) {
			glBindTexture(GL_TEXTURE_2D, rttex[i]);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GLuint attachments[Max] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
		}; 
		glDrawBuffers(Max, attachments );
	}

	void UnsetTexture() {
		//Resolve();
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/*
	   void Begin() {
	   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	   }

	   void End() {
	   Resolve();
	   glBindFramebuffer(GL_FRAMEBUFFER, 0);

	   }
	   */

};


#endif
