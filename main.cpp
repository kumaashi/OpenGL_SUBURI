//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
#include "include/util.h"
#include "include/view.h"
#include "include/rendertarget.h"
#include "include/mesh.h"
#include "include/camera.h"
#include "include/shader.h"

namespace {

	View         firstview;
	View         mainview;

	RenderTarget rt;
	RenderTarget rtdisp;

	Shader       mshader;
	Shader       rectshader;
	Shader       blitshader;
	Mesh         mesh;

	Camera       camera;
	Matrix       ctrlMatrix;
	vec          pos(0,10,-10);
	vec          at(0,0,0);
	vec          up(0,1,0);
	float        fFov     = 60.0;
	float        zNear    = 1.00;
	float        zFar     = 2560;
}

//ResetShader
void ResetShader() {
	mshader.LoadProgramFromFile("./res/vvs.fx",   "./res/gvs.fx", "./res/vfs.fx");
	rectshader.LoadProgramFromFile("./res/vrect.fx", NULL, "./res/frect.fx");
	blitshader.LoadProgramFromFile("./res/vblit.fx", NULL, "./res/fblit.fx");

	//FOR FAILED CASE 
	if(!mshader.Get())    WAIT(1000);
	if(!rectshader.Get()) WAIT(1000);
	if(!blitshader.Get()) WAIT(1000);
	printf("%s:done ResetShader\n", __FUNCTION__);
}

//Setup RenderTarget
void ResetRenderTarget() {
	rt.Create("RT", DEFAULT_WIDTH, DEFAULT_HEIGHT);
	rtdisp.Create("RTDISP", DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

//Setup Camera
void ResetCamera() {
	printf("%s : Call", __FUNCTION__);

	camera.Reset();
	camera.SetScreen(GetWidth(), GetHeight());
	camera.SetView(pos, at, up);
	camera.SetProj(fFov, zNear, zFar);
}

void Handle_WM_SIZE(int w, int h) {
	printf("%s : Call", __FUNCTION__);

	ResetCamera();
	ResetRenderTarget();
	firstview.SetRenderTarget(&rt);
}


//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void StartMain(int argc, char *argv[], HDC hdc) {
	Handle_WM_SIZE(GetWidth(), GetHeight());
	AddEvent_WM_SIZE(Handle_WM_SIZE);

	ResetShader();

	if(argc == 1) {
		mesh.Load(&mshader, "./res/cube.stl");
	} else {
		mesh.Load(&mshader, argv[1]);
	}

	static float g_time = 0;
	glSetInterval(1);
	while(ProcMsg()) {
		show_fps();
		static unsigned long start = timeGetTime();
		unsigned long delta = timeGetTime() - start;
		float dtime = float(delta) / 1000.0f;
		start = timeGetTime();

		//Update
		float r = 30;
		float speed = 0.3 * dtime;
		if(GetKey('S')) { ResetShader(); }
		if(GetKey('1')) { vec pos (r, r, -r); camera.SetTracking(pos, speed); }
		if(GetKey('2')) { vec pos (-r, r, -r); camera.SetTracking(pos, speed); }
		if(GetKey('A')) { vec pos (frand() * r, frand() * r, frand() * r); camera.SetTracking(pos, speed); }

		camera.Update();

		float info[4]  = {
			static_cast<float>(rt.GetWidth()), static_cast<float>(rt.GetHeight()),
			static_cast<float>(zNear), static_cast<float>(zFar)
		};

		float info2[4] = {
			static_cast<float>(GetWidth()), static_cast<float>(GetHeight()),
			static_cast<float>(g_time), static_cast<float>(g_time)
		};

		//Set Render Path
		if(1) {
			Matrix world;

			firstview.Bind();
			firstview.SetViewPort(0, 0, rt.GetWidth(), rt.GetHeight());
			firstview.ClearColor(0.0, 1.0, 0.5, 1.0);

			glEnable(GL_DEPTH_TEST);
			mesh.Begin();

			mshader.SetUniformMatrix4fv("view",  1, GL_FALSE, camera.GetView());
			mshader.SetUniformMatrix4fv("proj",  1, GL_FALSE, camera.GetProj());

			//RANDOM MOVE
			float begin  = 3;
			float margin = 2.2;
			static float ugoki = 0.0;
			ugoki += dtime;
			for(float z = -begin ;z < begin;z += margin) {
				for(float x = -begin ;x < begin;x += margin) {
					world.Ident();
					world.Trans(x, sin(3.0 * x + z + x * z + ugoki * 3.2) * 0.5, z);
					mshader.SetUniformMatrix4fv("world",  1, GL_FALSE, (float *)&world);
					mesh.Draw();
				}
			}
			mesh.End();

			firstview.Unbind();
		}

		//blit
		if(1)
		{
			glDisable(GL_DEPTH_TEST);

			//main view is not have RenderTarget
			mainview.Bind();
			mainview.SetViewPort(0, 0,  GetWidth(), GetHeight());
			mainview.ClearColor(0, 0, 1, 0);

			//Create Texture
			blitshader.Begin();
			blitshader.BindTexture("tex0", rt.GetTexture(0), 0);
			blitshader.BindTexture("tex1", rt.GetTexture(1), 1);
			blitshader.BindTexture("tex2", rt.GetTexture(2), 2);
			blitshader.BindTexture("tex3", rt.GetTexture(3), 3);
			glRects(-1, -1, 1, 1);
			blitshader.End();
			glEnable(GL_DEPTH_TEST);

			mainview.Unbind();
		}
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += (1.0 / 60.0);

		//DEBUG
		{
			GLint oldfbo = -1;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldfbo);
			printf("Old = %08X\n", oldfbo);
		}
	}
}


//--------------------------------------------------------------------------------------
// ep
//--------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	return Init(argc, argv, StartMain);
}

