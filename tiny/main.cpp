#include <windows.h>
#include <math.h>
#include <string>
#include <vector>

//OpenGL
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")




namespace Util {

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		int temp = wParam & 0xFFF0;
		switch(msg) {
		case WM_SYSCOMMAND:
			if(temp == SC_MONITORPOWER || temp == SC_SCREENSAVE) {
				return 0;
			}
			break;

		case WM_IME_SETCONTEXT:
			lParam &= ~ISC_SHOWUIALL;
			break;

		case WM_SIZE:
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE) {
				PostQuitMessage(0);
			}
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	struct Base {
		HWND  hWnd  ;
		HGLRC hGLRC ;
		HDC   hDC   ;
		int   Width ;
		int   Height;
		
		HWND GetHandle() {
			return hWnd;
		}

		void Term() {
			if(hGLRC) {
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(hGLRC);
				hGLRC = NULL;
			}
			if(hDC) {
				ReleaseDC(hWnd, hDC);
				hDC= NULL;
			}
		}

		void Init(const char *AppName, int w, int h) {
			hWnd          = NULL;
			hGLRC         = NULL;
			hDC           = NULL;
			Width         = w;
			Height        = h;
			WNDCLASSEX wcex = {
				sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
				WindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
				LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH),
				NULL, NULL, NULL
			};
			wcex.lpszClassName = AppName;

			RECT  rc      = {0, 0, Width, Height};
			DWORD style   = WS_OVERLAPPEDWINDOW;
			DWORD styleex = WS_EX_APPWINDOW;
			AdjustWindowRectEx(&rc, style, FALSE, styleex);
			rc.right     -= rc.left;
			rc.bottom    -= rc.top;

			RegisterClassEx(&wcex);
			hWnd = CreateWindowEx(
					   styleex, AppName, AppName, style,
					   (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2,
					   (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
					   rc.right, rc.bottom, 0, 0, wcex.hInstance, NULL);
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);

			hDC = GetDC(hWnd);

			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
				PFD_TYPE_RGBA, 32,
				0, 0,
				0, 0,
				0, 0,
				0, 0,
				0, 0, 0, 0, 0,
				32
			};

			SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
			hGLRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hGLRC);
		}

		void Present() {
			wglSwapLayerBuffers(hDC, WGL_SWAP_MAIN_PLANE);
		}

		int DoEvents() {
			MSG msg;
			while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				if(msg.message == WM_QUIT) return 0;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return 1;
		}
	};


	struct Fps {
		unsigned long last, frames;
		void Update() {
			unsigned long current = timeGetTime();
			frames++;
			if(1000 >= current - last) return;
			char  buf[256] = "";
			float dt  = (float)(current - last) / 1000.0f;
			float fps = (float)frames / dt;
			last = current;
			frames = 0;
			sprintf(buf, "%.02f fps", fps);
			
			printf("%s\n", buf);
		}
	};
	
	struct Random {
		int a, b, c;
		Random() { a = 1, b = 23456, c = 234566; }
		int GetInt() {
			a += b;
			b += c;
			c += a;
			return (a >> 16);
		}

		float GetFloat() {
			return (float(GetInt()) / float(0x7FFF));
		}
	};
	
	struct vec3 {
		union {
			float v[3];
			struct { float x, y, z; };
		};
		vec3()                              { x = y = z = 0.0f; }
		vec3(float a)                       { x = y = z = a; }
		vec3(float a, float b, float c)     { x = a; y = b; z = c; }
		vec3  operator +  (const vec3 &a)   { return vec3(x + a.x, y + a.y, z + a.z); }
		vec3  operator -  (const vec3 &a)   { return vec3(x - a.x, y - a.y, z - a.z); }
		vec3  operator *  (const vec3 &a)   { return vec3(x * a.x, y * a.y, z * a.z); }
		vec3  operator /  (const vec3 &a)   { return vec3(x / a.x, y / a.y, z / a.z); }
		vec3  operator +  (float a)         { return vec3(x + a, y + a, z + a); }
		vec3  operator -  (float a)         { return vec3(x - a, y - a, z - a); }
		vec3  operator *  (float a)         { return vec3(x * a, y * a, z * a); }
		vec3  operator /  (float a)         { return vec3(x / a, y / a, z / a); }
		vec3 &operator += (const vec3 &a)   { x += a.x; y += a.y; z += a.z; return *this; }
		vec3 &operator -= (const vec3 &a)   { x -= a.x; y -= a.y; z -= a.z; return *this; }
		vec3 &operator *= (const vec3 &a)   { x *= a.x; y *= a.y; z *= a.z; return *this; }
		vec3 &operator /= (const vec3 &a)   { x /= a.x; y /= a.y; z /= a.z; return *this; }
		vec3  operator >  (const vec3 &a)   { return vec3(x > a.x, y > a.y, z > a.z); }
		vec3  operator <  (const vec3 &a)   { return vec3(x < a.x, y < a.y, z < a.z); }
		float dot(vec3 &a)                  { return x * a.x + y * a.y + z * a.z; }
		vec3  cross(vec3 &a)                { return vec3( y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }
		float length()                      { return sqrt(dot(*this));}
		vec3 &normalize()                   { float l = 1.0f / length(); return *this = vec3(x * l, y * l, z * l); }
		float frac(float a)                 { a -= (float)(int)a; if(a < 0) return -a; return a; }
		vec3  frac()                        { return vec3(frac(x), frac(y), frac(z)); }
		vec3  abs()                         { return vec3(::abs(x), ::abs(y), ::abs(z)); }
		vec3  integer()                     { return vec3((float)(int)(x), (float)(int)(y), (float)(int)(z)); }
		vec3 &left()                        { float t = x; x = y; y = z; z = t; return *this;}
		vec3 &right()                       { float t = z; z = y; y = x; x = t; return *this;}
		DWORD color() {
			float r = x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x;
			float g = y > 1.0f ? 1.0f : y < 0.0f ? 0.0f : y;
			float b = z > 1.0f ? 1.0f : z < 0.0f ? 0.0f : z;
			return (int(r * 255) << 16) | (int(g * 255) << 8) | int(b * 255);
		}
		void  disp()                        { printf("%f %f %f \n", x, y, z); }
	};
	
	struct File {
		std::vector<unsigned char> buf;
		void *Buf() { return &buf[0]; }
		int Load(const char *name) {
			FILE *fp = fopen(name, "rb");
			if(fp) {
				fseek(fp, 0, SEEK_END);
				int size = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				buf.resize(size);
				fread(&buf[0], 1, size, fp);
				fclose(fp);
				return size;
			}
			return 0;
		}
	};
	
	/*
		Render <- Mesh <- Table
	*/
	struct Table {
		vec3 pos;
		vec3 scale;
		vec3 rotate;
		vec3 color;
		int mesh;
		int mode;
		void Reset() { memset(this, 0, sizeof(Table)); }
		void Apply() {
			glTranslatef(pos.x, pos.y, pos.z);
			glRotatef(rotate.x, 1.0f, 0.0f, 0.0f);
			glRotatef(rotate.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rotate.z, 0.0f, 0.0f, 1.0f);
			glScalef(scale.x, scale.y, scale.z);
			//glColor3f(color.x, color.y, color.z);
			float material[] = {
				color.x, color.y, color.z, 1.0
			};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
		}
	};

	struct Mesh {
		std::vector<float> vVertex;
		std::vector<float> vNormal;
		std::vector<float> vColor;
		int triangle_num;
		void Reset() { 
			vVertex.clear();
			vNormal.clear();
			vColor.clear();
			triangle_num = 0;
		}

		void Load(const char *name) {
			Random dummycolor;
			File f;
			int size = f.Load(name);
			if(!size) return;
			//STL
			unsigned char *buf = (unsigned char *)f.Buf();
			int index          = 80;
			triangle_num       = *(int *)(&buf[index]);
			index             += 4;
			int cnt            = 0;
			printf("trimax = %d\n", triangle_num);
			while(cnt < triangle_num) {
				#pragma pack(push,1)
				struct stldata {
					float n[3];
					float p[9];unsigned short reserve;
				};
				#pragma pack(pop)
				stldata d = *(stldata *)(&buf[index]); 
				for(int j = 0 ; j < 3; j++) {
					for(int i = 0 ; i < 3; i++) {
						vNormal.push_back(d.n[i]);
					}
				}
				float c = dummycolor.GetFloat();
				for(int i = 0 ; i < 9; i++) {
					vVertex.push_back(d.p[i]);
					vColor.push_back(dummycolor.GetFloat());
				}
				index += sizeof(stldata);
				cnt++;
			}
		}
		float *GetVertex() { return &vVertex[0]; }
		float *GetNormal() { return &vNormal[0]; }
		float *GetColor() { return &vColor[0]; }
		int GetSize() { return triangle_num; }
	};

	struct Render {
		std::vector<Table> vTable;
		std::vector<Mesh>  vMesh;
		int tableindex;
		enum {
			TableMax = 32768,
			MeshMax = 8,
		};
		
		void Init() {
			vTable.resize(TableMax);
			vMesh.resize(MeshMax);
			for(int i = 0; i < TableMax; i++) {
				vTable[i].Reset();
			}
			for(int i = 0; i < MeshMax; i++) {
				vMesh[i].Reset();
			}
		}
		
		void LoadMesh(int i, const char *name) {
			if(i > 0) {
				vMesh[i].Load(name);
			}
		}

		void Clear(float r, float g, float b, float a) {
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void SetState() {
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_DEPTH_TEST);
			//glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			//glEnable(GL_BLEND);
			
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);
			//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		}

		void Reset(float w, float h) {
			tableindex = 0;

			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(30.0, (double)w / (double)h, 0.01, 2560.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			SetState();
		}

		void LookAt(vec3& pos, vec3& at, vec3& up) {
			gluLookAt(
				pos.x, pos.y, pos.z,
				at.x, at.y, at.z,
				up.x, up.y, up.z);
		}

		void Add(int mesh, vec3 &p, vec3 &r, vec3 &s, vec3 &c) {
			Table *w = &vTable[tableindex++];
			w->mesh   = mesh;
			w->pos    = p;
			w->rotate = r;
			w->scale  = s;
			w->color  = c;
		}

		void Flush() {
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			int tablemesh = 0;
			for(int i = 0; i < tableindex; i++) {
				if( (i % 1024) == 0) {
					glFlush();
				}
				int mesh = vTable[i].mesh;
				int size = vMesh[mesh].GetSize() * 3;
				if(mesh) {
					float *pos = vMesh[mesh].GetVertex();
					float *nor = vMesh[mesh].GetNormal();
					float *col = vMesh[mesh].GetColor();
					if(mesh != tablemesh) {
						glVertexPointer(3, GL_FLOAT, 0, pos);
						glNormalPointer(GL_FLOAT, 0, nor);
						glColorPointer(3, GL_FLOAT, 0, col);
						tablemesh = mesh;
						size = vMesh[tablemesh].GetSize() * 3;
					}
					glPushMatrix();
					vTable[i].Apply();
					glDrawArrays(GL_TRIANGLES, 0, size);
					glPopMatrix();
				}
			}
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		};
	};
};


int main() {
	Util::Fps fps;
	Util::Base base;
	Util::Render render;
	int Width = 1280;
	int Height = 720;
	base.Init("UNKO", Width, Height);
	render.Init();
	render.LoadMesh(1, "cube.stl");
	float angle = 0;
	while(base.DoEvents()) {
		using namespace Util;
		//render.Clear(0, 0, 0, 0);
		render.Clear(1, 1, 1, 1);
		render.Reset(Width, Height);
		//vec3 pos = vec3(0, 5, -10);
		angle += 0.016666;
		vec3 pos = vec3(cos(angle) * 64, 5, sin(angle) * 64);
		vec3 at  = vec3(0, 0,  0);
		vec3 up  = vec3(0, 1,  0);
		render.LookAt(pos, at, up);
		for(int i = 0; i < 10; i++) {
			float x =  cos(float(i + 3.6) * 2.3) * 10;
			float y =  sin(float(i + 4.6) * 4.5) * 10;
			float z = -cos(float(i + 5.6) * 3.6) * 10;
			render.Add(1, vec3(x, y, z), vec3(x, y, z), vec3(1, 1, 1), vec3(0.3, 0.1, 0.05));
		}
		render.Flush();
		base.Present();
		fps.Update();
	}
	base.Term();
	return 0;
}

