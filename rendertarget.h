#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "util.h"

//------------------------------------------------------------------------------------------
// RenderTarget
//------------------------------------------------------------------------------------------
struct RenderTarget {
	GLuint fbo;
	GLuint fbo2;
	GLuint rttex;
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
			GL_DEBUG;
		}
	}
	
	void Create(int w, int h, int ms = 8) {
		printf("%s: Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, w, h, ms);
		int status = 0;
		glGenTextures(1, &rttex);  //1ST Create Texture
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex);
		GL_DEBUG;
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
		GL_DEBUG;
		glSetupState();
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;
		
		glGenFramebuffers(1, &fbo);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GL_DEBUG;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, rttex, 0);
		GL_DEBUG;
		
		glGenRenderBuffers(1, &rbo);
		GL_DEBUG;
		glBindRenderBuffer(GL_RENDERBUFFER, rbo);
		GL_DEBUG;
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h);
		GL_DEBUG;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		GL_DEBUG;
		if((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
		} else {
			printf("Bind OK : %d, %d\n", rttex, fbo);
		}
		glBindRenderBuffer(GL_RENDERBUFFER, 0);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;

		//2ND
		glGenTextures(1, &rttex2);
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, rttex2);
		GL_DEBUG;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, 0);
		GL_DEBUG;
		//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
		glSetupState();
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;

		glGenFramebuffers(1, &fbo2);
		GL_DEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
		GL_DEBUG;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rttex2, 0);
		GL_DEBUG;
		if( (status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			printf("Bind Failed : status=%08X\n", status);
		} else {
			printf("Bind OK : %d, %d\n", rttex2, fbo2);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;
		printf("%s, DONE\n", __FUNCTION__);

		Width  = w;
		Height = h;
		Sample = ms;
	}

	void SetTexture() {
		glActiveTexture(GL_TEXTURE0);
		GL_DEBUG;
		glBindTexture(GL_TEXTURE_2D, rttex2);
		GL_DEBUG;
		glGenerateMipmap(GL_TEXTURE_2D);
		GL_DEBUG;
	}
	
	void UnsetTexture() {
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_DEBUG;
	}

	void Begin() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GL_DEBUGEx(fbo);
	}

	void End() {
		Resolve();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_DEBUG;
	}

	void Resolve() {
		glBindFramebuffer( GL_READ_FRAMEBUFFER, fbo);
		GL_DEBUG;
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo2);
		GL_DEBUG;
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		GL_DEBUG;
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0);
		GL_DEBUG;
		glBindFramebuffer( GL_READ_FRAMEBUFFER, 0);
		GL_DEBUG;
	}
};


#endif
