//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
#include "util.h"

//TEST Shader
GLuint shader      = 0;
GLuint shader_rect = 0;
GLuint shader_blit = 0;

void ResetShader() {
	//shader      = glLoadShader("./res/vvs.fx",   "./res/gvs.fx", "./res/vfs.fx");
	shader      = glLoadShader("./res/vvs.fx",   NULL, "./res/vfs.fx");
	shader_rect = glLoadShader("./res/vrect.fx", NULL, "./res/frect.fx");
	shader_blit = glLoadShader("./res/vblit.fx", NULL, "./res/fblit.fx");
	printf("%s : Done %d %d %d\n", __FUNCTION__, shader, shader_rect, shader_blit);
}

//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void StartMain(int argc, char *argv[], HDC hdc) {
	RenderTarget rt;
	RenderTarget rtdisp;
	Mesh         mesh;
	Camera       camera;

	if(argc == 1) {
		mesh.Load("./res/cube.stl");
	} else {
		mesh.Load(argv[1]);
	}

	//const 
	vec pos (0,10,-10);
	vec at  (0,0,0);
	vec up  (0,1,0);
	float fFov     = 60.0;
	float zNear    = 1.00;
	float zFar     = 2560;

	camera.Reset();
	camera.SetScreen(Width, Height);
	camera.SetView(pos, at, up);
	camera.SetProj(fFov, zNear, zFar);

	rt.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	rtdisp.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	// START !!!!!
	static float g_time = 0;
	ResetShader();
	while(ProcMsg()) {
		//show_fps();
		
		//Create Delta
		static unsigned long start = timeGetTime();
		unsigned long delta = timeGetTime() - start;
		float dtime = float(delta) / 1000.0f;
		start = timeGetTime();

		//Update
		float r = 30;
		float speed = 0.3 * dtime;
		if(GetAsyncKeyState(VK_F5) & 0x8000) { ResetShader(); }
		if(GetAsyncKeyState('1') & 0x8000) { vec pos (r, r, -r); camera.SetTracking(pos, speed); }
		if(GetAsyncKeyState('2') & 0x8000) { vec pos (-r, r, -r); camera.SetTracking(pos, speed); }
		if(GetAsyncKeyState('A') & 0x8000) { vec pos (frand() * r, frand() * r, frand() * r); camera.SetTracking(pos, speed); }
		camera.Update();

		float info[4]  = {
			static_cast<float>(rt.Width),
			static_cast<float>(rt.Height),
			static_cast<float>(zNear),
			static_cast<float>(zFar)
		};
		GLint ulocinfo1 = glGetUniformLocation(shader, "info");

		float info2[4] = {
			static_cast<float>(Width),
			static_cast<float>(Height),
			static_cast<float>(g_time),
			static_cast<float>(g_time)
		};
		GLint ulocinfo2 = glGetUniformLocation(shader, "info2");

		//Set Render Path
		glClearColor(1.0, 0.0, 0.5, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if(1) {
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0);
			//rt.Begin();
			Matrix world;
			mesh.SetShader(shader);
			mesh.Bind();
			glViewport(0, 0, rt.Width, rt.Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUniform4fv(ulocinfo1, 1,  info);
			glUniform4fv(ulocinfo2, 1, info2);
			glUniformMatrix4fv(glGetUniformLocation(shader, "view"),  1, GL_FALSE, camera.GetView());
			glUniformMatrix4fv(glGetUniformLocation(shader, "proj"),  1, GL_FALSE, camera.GetProj());

			//RANDOM MOVE
			srand(0);
			float begin  = 50;
			float margin = 2.2;
			static float ugoki = 0.0;
			ugoki += dtime;
			int loc = glGetUniformLocation(shader, "world");
			for(float z = -begin ;z < begin;z += margin) {
				for(float x = -begin ;x < begin;x += margin) {
					world.Ident();
					world.Trans(x, sin(3.0 * x + z + x * z + ugoki * 3.2) * 0.5, z);
					glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)&world);
					mesh.Draw();
				}
			}
			mesh.Unbind();
			//rt.End();
		}

		if(1) {
			rtdisp.Begin();
			glViewport(0, 0, rt.Width, rt.Height);
			glDisable(GL_DEPTH_TEST);
			glUseProgram(shader_rect);
			rt.SetTexture();
			glUniform1i(glGetUniformLocation(shader_rect,  "tex"),   0);
			glUniform4fv(glGetUniformLocation(shader_rect, "info"),  1, info);
			glUniform4fv(glGetUniformLocation(shader_rect, "info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			rt.UnsetTexture();
			glEnable(GL_DEPTH_TEST);
			rtdisp.End();
			glUseProgram(0);
		}

		//blit
		if(1) {
			glUseProgram(shader_blit);
			glViewport(0, 0, Width, Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			rtdisp.SetTexture();
			glUniform1i(glGetUniformLocation(shader_blit,  "tex"), 0);
			glUniform4fv(glGetUniformLocation(shader_blit, "info"), 1, info);
			glUniform4fv(glGetUniformLocation(shader_blit, "info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			rtdisp.UnsetTexture();
			glEnable(GL_DEPTH_TEST);
			glUseProgram(0);
		}
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += (1.0 / 60.0);
	}
}


//----------:---------------------------------------------------------------------------
// ep
//--------------------------------------------------------------------------------------
int main(int argc, char *argv[]) { return Init(argc, argv, StartMain);}

