//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
#include "util.h"
#include "rendertarget.h"
#include "mesh.h"
#include "camera.h"

namespace {
	
	Shader       mshader;
	Shader       rectshader;
	Shader       blitshader;
	Mesh         mesh;
	
	RenderTarget rt;
	RenderTarget rtdisp;
	
	Camera       camera;
	Matrix       ctrlMatrix;
	vec          pos(0,10,-10);
	vec          at(0,0,0);
	vec          up(0,1,0);
	float        fFov     = 60.0;
	float        zNear    = 1.00;
	float        zFar     = 2560;
}

void ResetShader() {
	//mshader      = LoadProgramFromFile("./res/vvs.fx",   "./res/gvs.fx", "./res/vfs.fx");
	mshader.LoadProgramFromFile("./res/vvs.fx",   NULL, "./res/vfs.fx");
	rectshader.LoadProgramFromFile("./res/vrect.fx", NULL, "./res/frect.fx");
	blitshader.LoadProgramFromFile("./res/vblit.fx", NULL, "./res/fblit.fx");
	if(!mshader.Get()) WAIT(1000);
	if(!rectshader.Get()) WAIT(1000);
	if(!blitshader.Get()) WAIT(1000);
	printf("DEBUG : done ResetShader\n");
}


void Handle_WM_SIZE(int w, int h) {
	//Setup Camera
	camera.Reset();
	camera.SetScreen(Width, Height);
	camera.SetView(pos, at, up);
	camera.SetProj(fFov, zNear, zFar);
	printf("DEBUG : done setup camera\n");
	
	rt.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	rtdisp.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}


//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void StartMain(int argc, char *argv[], HDC hdc) {
	Handle_WM_SIZE(Width, Height);
	AddEvent_WM_SIZE(Handle_WM_SIZE);
	ResetShader();
	
	if(argc == 1) {
		mesh.Load(&mshader, "./res/cube.stl");
	} else {
		mesh.Load(&mshader, argv[1]);
	}
	
	static float g_time = 0;
	while(ProcMsg()) {
		static unsigned long start = timeGetTime();
		unsigned long delta = timeGetTime() - start;
		float dtime = float(delta) / 1000.0f;
		start = timeGetTime();

		//Update
		float r = 30;
		float speed = 0.3 * dtime;
		if(GetAsyncKeyState('S') & 0x8000) { ResetShader(); }
		if(GetAsyncKeyState('1') & 0x8000) { vec pos (r, r, -r); camera.SetTracking(pos, speed); }
		if(GetAsyncKeyState('2') & 0x8000) { vec pos (-r, r, -r); camera.SetTracking(pos, speed); }
		if(GetAsyncKeyState('A') & 0x8000) { vec pos (frand() * r, frand() * r, frand() * r); camera.SetTracking(pos, speed); }
		
		camera.Update();

		float info[4]  = { static_cast<float>(rt.Width), static_cast<float>(rt.Height), static_cast<float>(zNear), static_cast<float>(zFar) };
		float info2[4] = { static_cast<float>(Width), static_cast<float>(Height), static_cast<float>(g_time), static_cast<float>(g_time) };

		//Set Render Path
		if(1) {
			rt.Begin();
			glViewport(0, 0, rt.Width, rt.Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			Matrix world;
			mesh.Begin();
			
			GLint locview  = mshader.GetUniformLocation("view");
			GLint locproj  = mshader.GetUniformLocation("proj");
			GLint locworld = mshader.GetUniformLocation("world");
			glUniformMatrix4fv(locview, 1, GL_FALSE, camera.GetView());
			glUniformMatrix4fv(locproj, 1, GL_FALSE, camera.GetProj());

			//RANDOM MOVE
			float begin  = 50;
			float margin = 2.2;
			static float ugoki = 0.0;
			ugoki += dtime;
			for(float z = -begin ;z < begin;z += margin) {
				for(float x = -begin ;x < begin;x += margin) {
					world.Ident();
					world.Trans(x, sin(3.0 * x + z + x * z + ugoki * 3.2) * 0.5, z);
					glUniformMatrix4fv(locworld, 1, GL_FALSE, (float *)&world);
					mesh.Draw();
				}
			}
			mesh.End();
			rt.End();
		}

		if(1)
		{
			rtdisp.Begin();
			glUseProgram(rectshader.Get());
			glViewport(0, 0, rt.Width, rt.Height);
			glDisable(GL_DEPTH_TEST);
			rt.SetTexture();
			glUniform1i( rectshader.GetUniformLocation("tex"),   0);
			glUniform4fv(rectshader.GetUniformLocation("info"),  1, info);
			glUniform4fv(rectshader.GetUniformLocation("info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			rt.UnsetTexture();
			glEnable(GL_DEPTH_TEST);
			rtdisp.End();
			glUseProgram(0);
		}

		//blit
		if(1)
		{
			glUseProgram(blitshader.Get());
			glViewport(0, 0, Width, Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			rtdisp.SetTexture();
			glUniform1i( blitshader.GetUniformLocation("tex"), 0);
			glUniform4fv(blitshader.GetUniformLocation("info"), 1, info);
			glUniform4fv(blitshader.GetUniformLocation("info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			rtdisp.UnsetTexture();
			glEnable(GL_DEPTH_TEST);
			glUseProgram(0);
		}
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += (1.0 / 60.0);
		//Sleep(16);
	}
}


//--------------------------------------------------------------------------------------
// ep
//--------------------------------------------------------------------------------------
int main(int argc, char *argv[]) { return Init(argc, argv, StartMain);}

