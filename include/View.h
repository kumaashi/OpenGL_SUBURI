#ifndef _VIEW_H_
#define _VIEW_H_

#include "rendertarget.h"

//View <- RenderTarget
class View {
	float ccol[4];
	float viewinfo[4];
	float vp[4];
	RenderTarget *rt;
public:
	View() {
		memset(viewinfo, 0, sizeof(viewinfo));
		rt = NULL;
	}

	~View() {}
	
	float *GetViewPort()   { return vp; }
	float *GetClearColor() { return ccol; }

	void SetRenderTarget(RenderTarget *rendertarget) {
		rt = rendertarget;
	}
	

	void CreateRenderTarget() {
		//todo
	}

	void SetViewPort(int x0, int y0, int x1, int y1) {
		vp[0] = x0;
		vp[1] = y0;
		vp[2] = x1;
		vp[3] = y1;
	}
	
	void SetClearColor(float r, float g, float b, float a) {
		ccol[0] = r;
		ccol[1] = g;
		ccol[2] = b;
		ccol[3] = a;
	}

	void Begin() {
		if(rt) {
			rt->Begin();
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glViewport(vp[0], vp[1], vp[2], vp[3]);
		glClearColor( ccol[0], ccol[1], ccol[2], ccol[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void End() {
		if(rt) {
			rt->End();
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		
		//need?
		glFlush();
	}
};

#endif //_VIEW_H_
