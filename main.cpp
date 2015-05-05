//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
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
	void SetScreen(int w, int h) { Width = w, Height = h; }
	void SetView(vec &p, vec &a, vec &u) {
		if(!state) {
			Pos = p;
		}
		At  = a;
		Up  = u;
	}
	void SetProj(float fov, float fn, float ff) {
		ffov = fov, fnear = fn, ffar = ff;
	}
	float *GetView() { return (float *)&view; }
	float *GetProj() { return (float *)&proj; }
	void SetTracking(vec &p, float dt, int ty = 0) {
		Reset();
		PosStart = Pos;
		PosEnd   = p;
		dtt      = dt;
		state    = 1;
	}
	
	void Reset() {
		state = 0;
		tt    = 0;
	}
	
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

void StartMain(int argc, char *argv[], HDC hdc) {
	GLuint shader      = glLoadShader("vvs.cpp", "vfs.cpp");
	GLuint shader_rect = glLoadShader("vrect.cpp", "frect.cpp");
	
	Mesh         mesh;
	Texture3D    tex;
	RenderTarget rt;
	RenderTarget rtd;
	Camera       camera;
	
	rt.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	rtd.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	tex.Create();

	if(argc == 1) {
		mesh.Load("cube.stl");
	} else {
		mesh.Load(argv[1]);
	}
	vec pos (0,10,-10);
	vec at  (0,0,0);
	vec up  (0,1,0);
	float fFov     = 60.0;
	float zNear    = 0.1;
	float zFar     = 2560;
	camera.Reset();
	camera.SetScreen(Width, Height);
	camera.SetView(pos, at, up);
	camera.SetProj(fFov, zNear, zFar);
	static float g_time = 0;
	while(ProcMsg()) {
		if(GetAsyncKeyState(VK_F5) & 0x8000) {
			shader      = glLoadShader("vvs.cpp", "vfs.cpp");
			shader_rect = glLoadShader("vrect.cpp", "frect.cpp");
		}

		float info[4]  = {rt.Width, rt.Height, zNear, zFar};
		float info2[4] = {Width, Height, g_time, g_time};
		
		float r = 5;
		float speed = 0.003;
		//vec pos (frand() * r, frand() * r, frand() * r);
		if(GetAsyncKeyState('2') & 0x8000) {
			vec pos (-r, r, -r);
			camera.SetTracking(pos, speed);
		}

		if(GetAsyncKeyState('1') & 0x8000) {
			vec pos (r, r, -r);
			camera.SetTracking(pos, speed);
		}
		
		//Update
		camera.Update();
		{
			rt.Begin();
			glViewport(0, 0, rt.Width, rt.Height);
			glClearColor(0.25, 0.25, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glUseProgram(shader);
			tex.Bind();
			glUniform4fv(glGetUniformLocation(shader, "info"), 1, info);
			glUniform4fv(glGetUniformLocation(shader, "info2"), 1, info2);
			glUniformMatrix4fv(glGetUniformLocation(shader, "view"),  1, GL_FALSE, camera.GetView());
			glUniformMatrix4fv(glGetUniformLocation(shader, "proj"),  1, GL_FALSE, camera.GetProj());
			
			Matrix world;
			GLint loc = glGetUniformLocation(shader, "world");
			mesh.Bind();
			srand(0);
			for(float z = -30 ; z < 30; z += 2.05) {
				for(float x = -30 ; x < 30; x += 2.05) {
					world.Ident();
					//world.Trans(x, (float(rand()) / float(0x7FFF)) * 2.0 + sin(z + g_time) * 0.5, z);
					world.Trans(x, sin(3.0 * x + z + x * z + g_time * 3.2) * 0.5, z);
					glUniformMatrix4fv(loc, 1, GL_FALSE, (float *)&world);
					mesh.Draw();
				}
			}
			/*
			world.Ident();
			glUniformMatrix4fv(glGetUniformLocation(shader, "world"), 1, GL_FALSE, (float *)&world);
			mesh.Draw();
			*/
			rt.End();
		}

		//AO
		/*
		{
			glClearColor(0.55, 0.25, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, Width, Height);
			glDisable(GL_DEPTH_TEST);
			glUseProgram(shader_rect);
			rt.SetTexture();
			glUniform1i(glGetUniformLocation(shader_rect,  "tex"), 0);
			glUniform4fv(glGetUniformLocation(shader_rect, "info"), 1, info);
			glUniform4fv(glGetUniformLocation(shader_rect, "info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			glEnable(GL_DEPTH_TEST);
		}
		*/

		//AO
		{
			rtd.Begin();
			glClearColor(0.55, 0.25, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//glViewport(0, 0, Width, Height);
			glViewport(0, 0, rt.Width, rt.Height);
			glDisable(GL_DEPTH_TEST);
			glUseProgram(shader_rect);
			rt.SetTexture();
			glUniform1i(glGetUniformLocation(shader_rect,  "tex"), 0);
			glUniform4fv(glGetUniformLocation(shader_rect, "info"), 1, info);
			glUniform4fv(glGetUniformLocation(shader_rect, "info2"), 1, info2);
			glRects(-1, -1, 1, 1);
			glEnable(GL_DEPTH_TEST);
			rtd.End();
		}
		
		//blit
		glViewport(0, 0, Width, Height);
		rtd.Resolve(Width, Height);
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += 0.0166666666666666666666666666666666666;
		//Sleep(16);
	}
}



int main(int argc, char *argv[]) {
	return Init(argc, argv, StartMain);
}
