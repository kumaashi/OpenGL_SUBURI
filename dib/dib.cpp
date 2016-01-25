//--------------------------------------------------------------------------------------
//
// DIVB
//
//
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

//--------------------------------------------------------------------------------------
// pragma
//--------------------------------------------------------------------------------------
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")

//--------------------------------------------------------------------------------------
// define
//--------------------------------------------------------------------------------------
#define u8   unsigned char
#define u16  unsigned short
#define u32  unsigned long
#define s8   signed   char
#define s16  signed   short
#define s32  signed   long
#define f32  float
#define f64  double

//--------------------------------------------------------------------------------------
// vec3
//--------------------------------------------------------------------------------------
struct vec3 {
	union
	{
		float v[3];
		struct { float x, y, z; };
	};
	vec3()                              { x = y = z = 0.0f; }
	vec3(float a)                       { x = y = z = a; }
	vec3(float a, float b, float c)     { x = a;    y = b;    z = c;  }
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
	vec3  abs()                         { return vec3(::fabs(x), ::fabs(y), ::fabs(z)); }
	vec3  integer()                     { return vec3((float)(int)(x), (float)(int)(y), (float)(int)(z)); }
	vec3 &left()                        { float t = x; x = y; y = z; z = t; return *this;}
	vec3 &right()                       { float t = z; z = y; y = x; x = t; return *this;}
	DWORD color()
	{
		float r = x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x;
		float g = y > 1.0f ? 1.0f : y < 0.0f ? 0.0f : y;
		float b = z > 1.0f ? 1.0f : z < 0.0f ? 0.0f : z;
		return (int(r * 255) << 16) | (int(g * 255) << 8) | int(b * 255);
	}
	void  disp()                        { printf("%f %f %f \n", x, y, z); }
};



//--------------------------------------------------------------------------------------
//
// window  proc
//
//--------------------------------------------------------------------------------------
static LRESULT WINAPI SimpleMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
  switch( msg ) {
    case WM_SYSCOMMAND:
      switch( ( wParam & 0xFFF0 ) ) {
        case SC_MONITORPOWER:
        case SC_SCREENSAVE:
          return 0;
          break;
      }
      break;
    case WM_CLOSE:
    case WM_DESTROY:
      PostQuitMessage( 0 );
      return 0;
      break;
    case WM_IME_SETCONTEXT:
      lParam &= ~ISC_SHOWUIALL;
      break;
    case WM_KEYDOWN:
      if(wParam == VK_ESCAPE) PostQuitMessage(0);
      break;
  }
  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//--------------------------------------------------------------------------------------
//
// Event
//
//--------------------------------------------------------------------------------------
struct Event {
	int Update() {
		MSG msg;
		int done = 0;
		while(PeekMessage(&msg , 0 , 0 , 0 , PM_REMOVE)) {
			if(msg.message == WM_QUIT) {
				done = 1;
				break;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		return done;
	}
};


//--------------------------------------------------------------------------------------
//
// Video
//
//--------------------------------------------------------------------------------------
struct Video {
	int Width, Height, Bits;
	void        *pBuffer;
	int          FlipCount;
	float       *Buffer;
	HWND         hWnd;
	HINSTANCE    hInst;

	BITMAPINFO   bi;
	HBITMAP      hDIB;
	HDC          hDIBDC;
	DWORD        dwWidth  ;
	DWORD        dwHeight ;
	
	Video(const char *name, int width, int height, int bits) {
		Width             = width;
		Height            = height;
		float screenratio = float(Height) / float(Width);
		Bits              = bits;
		FlipCount         = 0;
		Buffer            = NULL;
		hWnd              = NULL;
		hInst             = GetModuleHandle(NULL);

		DWORD style       = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
		DWORD dwExStyle   = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		DWORD sysX        = GetSystemMetrics( SM_CXSCREEN );
		DWORD sysY        = GetSystemMetrics( SM_CYSCREEN );
		dwWidth           = sysX * screenratio;
		dwHeight          = sysY * screenratio;

		POINT point       = { (sysX - (LONG)dwWidth ) / 2, (sysY - dwHeight) / 2 };
		RECT rc = {0, 0, dwWidth, dwHeight};
		BITMAPINFO   bi;

		WNDCLASSEX twc =
		{
			sizeof( WNDCLASSEX ), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW, SimpleMsgProc, 0L, 0L,
				hInst,  LoadIcon(NULL, IDI_APPLICATION), LoadCursor( NULL , IDC_ARROW ), 
				( HBRUSH )GetStockObject( BLACK_BRUSH ), NULL, name, NULL
		};
		AdjustWindowRectEx(&rc, style, FALSE, dwExStyle);
		RegisterClassEx(&twc);
		hWnd = CreateWindowEx(dwExStyle, name, name, style, point.x, point.y, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL );
		//hWnd = CreateWindowEx(dwExStyle, name, name, style, 0, 0, Width, Height, NULL, NULL, hInst, NULL );
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth       = Width;
		bi.bmiHeader.biHeight      = -Height;
		bi.bmiHeader.biPlanes      = 1;
		bi.bmiHeader.biBitCount    = Bits;
		bi.bmiHeader.biCompression = BI_RGB;

		hDIB = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void **)&pBuffer, 0, 0);
		memset(pBuffer, 0xFF, Width * Height * 4);
		hDIBDC = CreateCompatibleDC(NULL);
		SaveDC(hDIBDC);
		SelectObject(hDIBDC, hDIB);
	}

	~Video() {
		if(hDIB) DeleteObject(hDIB);
	}

	int GetWidth() {
		return Width;
	}

	int GetHeight() {
		return Height;
	}

	int GetPixelSize() {
		return (Bits / 8);
	}

	void SetPixelBuffer(float *buf) {
		Buffer = buf;
	}

	float Clamp(float a, float x, float y) {
		return (a > y) ? y : ( (a < x) ? x : a);
	}

	unsigned long GetPixel(int x, int y) {
		if(!Buffer) return 0xFF00FFFF;
		int offset = ((x % Width) + ( (y % Height) * Width)) * 4 ; //rgba32f
		
		u8 r = Clamp(Buffer[ offset + 0], 0.0, 1.0) * 0xFF;
		u8 g = Clamp(Buffer[ offset + 1], 0.0, 1.0) * 0xFF;
		u8 b = Clamp(Buffer[ offset + 2], 0.0, 1.0) * 0xFF;
		u8 a = Clamp(Buffer[ offset + 3], 0.0, 1.0) * 0xFF;

		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}

	void Update() {
		HDC hdc = GetDC(hWnd);
		//StretchBlt(hdc, 0, 0, dwWidth, dwHeight, hDIBDC, 0, 0, Width, Height, SRCCOPY);
		BitBlt(hdc, 0, 0, dwWidth, dwHeight, hDIBDC, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
	}
};



//--------------------------------------------------------------------------------------
//
// Input
//
//--------------------------------------------------------------------------------------
struct Input {
	unsigned long    keybits;
	enum {
		S_Up     = 1 << 0,
		S_Down   = 1 << 1,
		S_Left   = 1 << 2,
		S_Right  = 1 << 3,
		S_B0     = 1 << 4,
		S_B1     = 1 << 5,
		S_B2     = 1 << 6,
		S_Escape = 1 << 7,
	};
	Input() {
		keybits = 0;
	}
	
	~Input() {
	}

	int Up    () { return keybits & S_Up    ;  }
	int Down  () { return keybits & S_Down  ;  }
	int Left  () { return keybits & S_Left  ;  }
	int Right () { return keybits & S_Right ;  }
	int B0    () { return keybits & S_B0    ;  }
	int B1    () { return keybits & S_B1    ;  }
	int B2    () { return keybits & S_B2    ;  }
	int Escape() { return keybits & S_Escape;  }

	int GetKey(DWORD e) {
		return (GetAsyncKeyState(e) & 0x8000) ? 1 : 0;
	}
	
	int Update() {
		int done = 0;
		if(GetKey(VK_UP    )) keybits |= S_Up    ;
		if(GetKey(VK_DOWN  )) keybits |= S_Down  ;
		if(GetKey(VK_LEFT  )) keybits |= S_Left  ;
		if(GetKey(VK_RIGHT )) keybits |= S_Right ;
		if(GetKey('Z'     )) keybits |= S_B0    ;
		if(GetKey('X'     )) keybits |= S_B1    ;
		if(GetKey('C'     )) keybits |= S_B2    ;

		if(GetKey(VK_ESCAPE)) {
			keybits |= S_Escape;
			done = 1;
		}
		return done;
	}

};





void ShowFps()
{
	//fps : http://www.t-pot.com/program/13_fps/index.html
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



int main()
{
	Video video("test", 320, 240, 32);
	Input input;
	Event event;
	while(input.Update() != 1) {
		if(event.Update()) break;
		video.Update();
		ShowFps();
	}
	return 0;
}
