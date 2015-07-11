//--------------------------------------------------------------------------------------
//
// main.cpp
//
//--------------------------------------------------------------------------------------
#include "util.h"

//--------------------------------------------------------------------------------------
// Data
//--------------------------------------------------------------------------------------

namespace Base {
class Base {
public:
	int id;
	vec pos, rotate, scale, color;
	Base() { id = 0; }
	int GetId() { return id; }
	int IsDeath() { return id <= 0; }
	int SetId(int i) { return id = i; }
	virtual void Update(){};
	virtual void Render(){};
};
std::vector<Base *> vbase;

bool func_sort(Base *a, Base *b) { return a->GetId() < b->GetId(); }


void Update() {
	std::sort(vbase.begin(), vbase.end(), func_sort);
	auto it = vbase.begin();
	auto ite = vbase.end();
	while(it != ite) {
		(*it)->Update();
		it++;
	}
}

void Render() {
	std::sort(vbase.begin(), vbase.end(), func_sort);
	auto it = vbase.begin();
	auto ite = vbase.end();
	while(it != ite) {
		(*it)->Render();
		it++;
	}
}

void Gc() {
	std::vector<Base *> vtemp;
	auto it = vbase.begin();
	auto ite = vbase.end();
	while(it != ite) {
		auto next = it + 1;
		if(!(*it)->IsDeath()) {
			vtemp.push_back(*it);
		}
		it = next;
	}
	vbase = vtemp;
}

void Add(int id, vec &p, vec &r, vec &s, vec &c) {
}

} //base

void StartMain(int argc, char *argv[], HDC hdc) {
	GLuint shader      = glLoadShader("vvs.cpp",   "grect.cpp", "vfs.cpp");
	printf("Create Render Target\n");
	GLuint shader_rect = glLoadShader("vrect.cpp", NULL, "frect.cpp");
	printf("Create Render Target\n");
	GLuint shader_blit = glLoadShader("vblit.cpp", NULL, "fblit.cpp");
	Mesh         mesh;
	RenderTarget rt;
	RenderTarget rtdisp;
	Camera       camera;

	printf("Create Render Target\n");
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
			//shader      = glLoadShader("vvs.cpp",   "grect.cpp", "vfs.cpp");
			//shader_rect = glLoadShader("vrect.cpp", "grect.cpp", "frect.cpp");
			//shader_blit = glLoadShader("vblit.cpp", "grect.cpp", "fblit.cpp");
			shader      = glLoadShader("vvs.cpp",   "grect.cpp", "vfs.cpp");
			shader_rect = glLoadShader("vrect.cpp", NULL, "frect.cpp");
			shader_blit = glLoadShader("vblit.cpp", NULL, "fblit.cpp");
			
		}

		float info[4]  = {rt.Width, rt.Height, zNear, zFar};
		float info2[4] = {Width, Height, g_time, g_time};
		float r = 30;
		float speed = 0.3 * dtime;
		printf("%5.6f\r", dtime);
		Base::Update();

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

		//Set Render Path
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

		{
			rtdisp.Begin(); GL_DEBUG1;
			glViewport(0, 0, rt.Width, rt.Height); GL_DEBUG1;
			glDisable(GL_DEPTH_TEST); GL_DEBUG1;
			glUseProgram(shader_rect); GL_DEBUG1;
			rt.SetTexture(); GL_DEBUG1;


			int loc;
			loc = glGetUniformLocation(shader_rect,  "tex");
			glUniform1i(loc, 0); GL_DEBUG1;
			loc = glGetUniformLocation(shader_rect, "info");
			glUniform4fv(loc, 1, info); GL_DEBUG1;
			loc = glGetUniformLocation(shader_rect, "info2");
			glUniform4fv(loc, 1, info2); GL_DEBUG1;
			glRects(-1, -1, 1, 1); GL_DEBUG1;
			rt.UnsetTexture(); GL_DEBUG1;
			glEnable(GL_DEPTH_TEST);
			rtdisp.End(); GL_DEBUG1;
		}

		//blit
		{
			glViewport(0, 0, Width, Height); GL_DEBUG1;
			glClearColor(0.25, 0.25, 0.5, 0); GL_DEBUG1;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_DEBUG1;
			glBindFramebuffer(GL_FRAMEBUFFER, 0); GL_DEBUG1;
			glDisable(GL_DEPTH_TEST); GL_DEBUG1;
			glUseProgram(shader_blit); GL_DEBUG1;
			rtdisp.SetTexture(); GL_DEBUG1;
			float info[4]  = {Width, Height, zNear, zFar};
			glUniform1i(glGetUniformLocation(shader_blit,  "tex"), 0); GL_DEBUG1;
			glUniform4fv(glGetUniformLocation(shader_blit, "info"), 1, info); GL_DEBUG1;
			glUniform4fv(glGetUniformLocation(shader_blit, "info2"), 1, info2); GL_DEBUG1;
			glRects(-1, -1, 1, 1); GL_DEBUG1;
			rt.UnsetTexture(); GL_DEBUG1;
			glEnable(GL_DEPTH_TEST); GL_DEBUG1;
		}
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
		g_time += 0.0166666666666666666666666666666666666;
	}
}



int main(int argc, char *argv[]) {
	return Init(argc, argv, StartMain);
}

