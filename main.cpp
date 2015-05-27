//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
#include "util.h"

//--------------------------------------------------------------------------------------
// Data
//--------------------------------------------------------------------------------------
struct Data {
	enum {
		TYPE_NONE,
		TYPE_CAMERA,
		TYPE_CH0,
		TYPE_CH_MAX = 256,
	};
	int type;
	struct {
		vec trans;
		vec rotate;
		vec scale;
		vec color;
	};

	int IsNone() { return type == TYPE_NONE; }
	int Del() {
		type = TYPE_NONE;
	}
};


//--------------------------------------------------------------------------------------
// Scene
//--------------------------------------------------------------------------------------
struct DataPool {

	std::vector<Data> vdata;
	Data dummy;
	int dataindex;
	int datamax;
	void Init(int m) {
		vdata.resize(m);
		dataindex = 0;
		datamax = m;
	}

	Data *Get(int type) {
		Data *w = &vdata[dataindex++];
		if(w->IsNone()) return w;
		for(int i = 0 ; i < datamax; i++) {
			w = &vdata[i];
			if(w->IsNone()) return w;
		}
		printf("Scene return dummy[BUG]\n");
		return &dummy;
	}
};

//--------------------------------------------------------------------------------------
// Scene
//--------------------------------------------------------------------------------------
bool data_sort(const Data *l, const Data *r) {
	return l->type < r->type;
}

struct Scene {
	std::vector<Data *> vData;
	void Render() {
		Matrix *view = NULL;
		Matrix *proj = NULL;
		
		std::sort(vData.begin(), vData.end(), data_sort);
		auto ite  = vData.begin();
		auto eite = vData.end();
		while(ite != eite) {
			/*
			if((*ite)->type == Data::TYPE_CAMERA) {
				view = (*ite)->camera.GetView();
				proj = (*ite)->camera.GetProj();
				continue;
			}
			*/
			/*
			glUniformMatrix4fv(glGetUniformLocation(program, "view"),	1, GL_FALSE, &view);
			glUniformMatrix4fv(glGetUniformLocation(program, "proj"),	1, GL_FALSE, &proj);
			*/
		}
	}

  void Update() {
    //update per data
  }
};


//fps : http://www.t-pot.com/program/13_fps/index.html
void show_fps() {
	static DWORD    last = timeGetTime();
	static DWORD    frames = 0;
	static char     buf[256] = "";
	DWORD           current;
	current = timeGetTime();
	frames++;
	if(1000 <= current - last) {
		float dt = (float)(current - last) / 1000.0f;
		float fps = (float)frames / dt;
		last = current;
		frames = 0;
		sprintf(buf, "%.02f fps", fps);
		printf("%s\n", buf);
	}
}



void StartMain(int argc, char *argv[], HDC hdc) {
	GLuint shader      = glLoadShader("vvs.cpp",   "vfs.cpp");
	GLuint shader_rect = glLoadShader("vrect.cpp", "frect.cpp");
	GLuint shader_blit = glLoadShader("vblit.cpp", "fblit.cpp");
	Mesh         mesh;
	RenderTarget rt;
	RenderTarget rtdisp;
	Camera       camera;
	
	rt.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	rtdisp.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	if(argc == 1) {
		mesh.Load("cube.stl");
	} else {
		mesh.Load(argv[1]);
	}
	vec pos (0,10,-10);
	vec at  (0,0,0);
	vec up  (0,1,0);
	float fFov     = 60.0;
	float zNear    = 0.01;
	float zFar     = 2560;
	camera.Reset();
	camera.SetScreen(Width, Height);
	camera.SetView(pos, at, up);
	camera.SetProj(fFov, zNear, zFar);
	
	printf("======================\n");
	printf("Press Key 1, 2, and A!\n");
	printf("======================\n");
	
	static float g_time = 0;
	while(ProcMsg()) {
		show_fps();
		static unsigned long start = timeGetTime();
		unsigned long delta = timeGetTime() - start;
		float dtime = float(delta) / 1000.0f;
		start = timeGetTime();
		if(GetAsyncKeyState(VK_F5) & 0x8000) {
			shader      = glLoadShader("vvs.cpp", "vfs.cpp");
			shader_rect = glLoadShader("vrect.cpp", "frect.cpp");
			shader_blit = glLoadShader("vblit.cpp", "fblit.cpp");
		}

		float info[4]  = {rt.Width, rt.Height, zNear, zFar};
		float info2[4] = {Width, Height, g_time, g_time};
		float r = 30;
		float speed = 0.3 * dtime;
		printf("%5.6f\r", dtime);

		if(GetAsyncKeyState('1') & 0x8000) {
			vec pos (r, r, -r);
			camera.SetTracking(pos, speed);
		}
		
		if(GetAsyncKeyState('2') & 0x8000) {
			vec pos (-r, r, -r);
			camera.SetTracking(pos, speed);
		}

		
		if(GetAsyncKeyState('A') & 0x8000) {
			vec pos (frand() * r, frand() * r, frand() * r);
			camera.SetTracking(pos, speed);
		}
		
		//Update
		camera.Update();

		glEnable( GL_MULTISAMPLE );
		{
			rt.Begin();
			glViewport(0, 0, rt.Width, rt.Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glUseProgram(shader);
			glUniform4fv(glGetUniformLocation(shader, "info"), 1, info);
			glUniform4fv(glGetUniformLocation(shader, "info2"), 1, info2);
			glUniformMatrix4fv(glGetUniformLocation(shader, "view"),  1, GL_FALSE, camera.GetView());
			glUniformMatrix4fv(glGetUniformLocation(shader, "proj"),  1, GL_FALSE, camera.GetProj());
			Matrix world;
			GLint loc = glGetUniformLocation(shader, "world");
			mesh.SetShader(shader);
			mesh.Bind();
			srand(0);
			float begin  = 50;
			float margin = 2.2;
			static float ugoki = 0.0;
			ugoki += dtime;
			for(float z = -begin ; z < begin; z += margin) {
				for(float x = -begin ; x < begin; x += margin) {
					world.Ident();
					world.Trans(x, sin(3.0 * x + z + x * z + ugoki * 3.2) * 0.5, z);
					glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)&world);
					mesh.Draw();
				}
			}
			mesh.Unbind();
			rt.End();
		}

		{
			rtdisp.Begin(); GL_DEBUG;
			glViewport(0, 0, rt.Width, rt.Height); GL_DEBUG;
			glDisable(GL_DEPTH_TEST); GL_DEBUG;
			glUseProgram(shader_rect); GL_DEBUG;
			rt.SetTexture(); GL_DEBUG;
			
			
			int loc;
			loc = glGetUniformLocation(shader_rect,  "tex");
			glUniform1i(loc, 0); GL_DEBUG;
			loc = glGetUniformLocation(shader_rect, "info");
			glUniform4fv(loc, 1, info); GL_DEBUG;
			loc = glGetUniformLocation(shader_rect, "info2");
			glUniform4fv(loc, 1, info2); GL_DEBUG;
			glRects(-1, -1, 1, 1); GL_DEBUG;
			rt.UnsetTexture(); GL_DEBUG;
			glEnable(GL_DEPTH_TEST);
			rtdisp.End(); GL_DEBUG;
		}
		
		//blit
		{
			glViewport(0, 0, Width, Height); GL_DEBUG;
			glClearColor(0.25, 0.25, 0.5, 0); GL_DEBUG;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_DEBUG;
			glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_DEBUG;
			glDisable(GL_DEPTH_TEST);GL_DEBUG;
			glUseProgram(shader_blit);GL_DEBUG;
			rtdisp.SetTexture();GL_DEBUG;
			float info[4]  = {Width, Height, zNear, zFar};
			glUniform1i(glGetUniformLocation(shader_blit,  "tex"), 0);GL_DEBUG;
			glUniform4fv(glGetUniformLocation(shader_blit, "info"), 1, info);GL_DEBUG;
			glUniform4fv(glGetUniformLocation(shader_blit, "info2"), 1, info2);GL_DEBUG;
			glRects(-1, -1, 1, 1);GL_DEBUG;
			rt.UnsetTexture();GL_DEBUG;
			glEnable(GL_DEPTH_TEST);GL_DEBUG;
		}
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += 0.0166666666666666666666666666666666666;
	}
}



int main(int argc, char *argv[]) {
	return Init(argc, argv, StartMain);
}
