#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

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
			//{ GL_GENERATE_MIPMAP,    GL_TRUE},
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

public:
	int  GetWidth() { return Width; }
	int  GetHeight() { return Width; }
	GLuint GetTexture(int index) {
		return rttex[index];
	}

	RenderTarget()  {
		fbo = 0;
	}

	~RenderTarget() {
		Release();
	}

	void Release() {
		if(fbo) {
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(Max, rbo);
			glDeleteRenderbuffers(1, &rbdepth);
			glDeleteTextures(Max, rttex);
			memset(rttex, 0, sizeof(rttex));
			memset(rbo, 0, sizeof(rbo));
			rbdepth = 0;
			fbo = 0;
			
		}
	}
	
	void Create(const char *name, int w, int h, int ms = 1) {
		Name = std::string(name);
		
		printf("%s: %s, Width=%d, Height=%d, Ms=%d\n", __FUNCTION__, name, w, h, ms);
		int status = 0;

		//Create Render Buffer and texture
		glGenFramebuffers(1, &fbo);
		glGenTextures(Max, rttex);
		glGenRenderbuffers(Max, rbo);
		glGenRenderbuffers(1, &rbdepth);

		//Bind
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//Color
		for(int i = 0 ; i < Max; i++) {
			glBindRenderBuffer(GL_RENDERBUFFER, rbo[i]);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_RGBA32F, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rbo[i]);
		}

		//Depth
		glBindRenderBuffer(GL_RENDERBUFFER, rbdepth);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, ms, GL_DEPTH_COMPONENT32, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbdepth);

		for(int i = 0 ; i < Max; i++) {
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rttex[i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, ms, GL_RGBA32F, w, h, GL_TRUE);
			glSetupState();
			glFramebufferTexture2D(
					GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, rttex[i], 0);
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
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderBuffer(GL_RENDERBUFFER, 0);
		Width  = w;
		Height = h;
		Sample = ms;
	}
	
	void Map()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		SetupDrawBuffers();
		printf("%s : %d\n", __FUNCTION__, fbo);
	}
	
	void Unmap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};


#endif

