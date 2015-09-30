#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "util.h"

//--------------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------------
struct Camera {
	Matrix view;
	Matrix proj;
	vec  Pos, At, Up;
	vec  PosStart;
	vec  PosEnd;
	float  tt;
	float dtt;
	int Width, Height;
	int state;
	float ffov, fnear, ffar;

	void SetScreen(int w, int h) {
		Width  = w;
		Height = h;
	}

	void SetView(vec &p, vec &a, vec &u) {
		if(!state) {
			Pos = p;
		}
		At  = a;
		Up  = u;
	}

	void SetProj(float fov, float fn, float ff) { ffov = fov, fnear = fn, ffar = ff; }
	float *GetView() { return (float *)&view; }
	float *GetProj() { return (float *)&proj; }

	void SetTracking(vec &p, float dt, int ty = 0) {
		Reset();
		PosStart = Pos;
		PosEnd   = p;
		dtt      = dt;
		state    = 1;
	}
	
	void Reset() { state = 0; tt    = 0; }
	void Update() {
		view.LookAt(Pos, At, Up);
		proj.Perspective(ffov, float(Width) / float(Height), fnear, ffar);
		if(state) {
			Pos.smoothstep(PosStart, PosEnd, tt);
			tt += dtt;
			if(tt > 1.0) {
				Pos   = PosEnd;
				tt    = 1.0;
				state = 0;
			}
		}
	}
};

#endif
