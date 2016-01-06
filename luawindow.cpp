#include "luamain.h"

namespace {
	const char *AppName = "dummy";
	int  Width          = 0;
	int  Height         = 0;
	HWND hWnd           = NULL;
	BOOL Windowed       = TRUE;
	int  xPos           = 0;
	int  yPos           = 0;
	BOOL mLeft          = FALSE;
	BOOL mRight         = FALSE;
	
	LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		int temp = wParam & 0xFFF0;
		switch(msg) {
		case WM_MOUSEMOVE:
			{
				xPos = GET_X_LPARAM(lParam);
				yPos = GET_Y_LPARAM(lParam);
			}
			break;
		case WM_LBUTTONDOWN: mLeft  = TRUE;  break;
		case WM_LBUTTONUP:   mLeft  = FALSE; break;
		case WM_RBUTTONDOWN: mRight = TRUE;  break;
		case WM_RBUTTONUP:   mRight = FALSE; break;
		case WM_SYSCOMMAND:
			if(temp == SC_MONITORPOWER || temp == SC_SCREENSAVE)
			{
				return 0;
			}
			break;
		case WM_IME_SETCONTEXT:
			lParam &= ~ISC_SHOWUIALL;
			break;
		case WM_SIZE:
			Width  = LOWORD(lParam);
			Height = HIWORD(lParam);
			/*
			if(ProcHandleResize) {
				ProcHandleResize(Width, Height);
			}
			*/
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_KEYDOWN:
			{
				if(wParam == VK_ESCAPE) PostQuitMessage(0);
				break;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	int RunApp(lua_State *l)
	{
		AppName = luaL_checkstring(l, 1);
		Width = (int)luaL_checknumber(l, 2);
		Height = (int)luaL_checknumber(l, 3);

		printf("%s : %s -> %d %d : ", __FUNCTION__, AppName, Width, Height);

		static WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
			WindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
			LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH),
			NULL, AppName, NULL
		};

		wcex.hInstance = GetModuleHandle(NULL);
		DWORD style   = Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
		DWORD styleex = WS_EX_APPWINDOW | (Windowed ? WS_EX_WINDOWEDGE : 0);
		
		RECT   rc;
		SetRect(&rc, 0, 0, Width, Height);
		AdjustWindowRectEx(&rc, style, FALSE, styleex);
		rc.right  -= rc.left;
		rc.bottom -= rc.top;
		if (!RegisterClassEx(&wcex))
		{
			printf("%s : failed RegisterClassEx\n", __FUNCTION__);
			LUARETURN_NIL(l, 0);
		}
		hWnd = CreateWindowEx(
		           styleex, AppName, AppName, style,
		           (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2,
		           (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
		           rc.right, rc.bottom, 0, 0, wcex.hInstance, 0);
		if(!hWnd)
		{
			printf("%s : failed CreateWindowEx\n", __FUNCTION__);
			LUARETURN_NIL(l, 0);
		}
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		printf(" Done.\n");

		LUARETURN(l, 1);
	}

	int ProcMsg(lua_State *l)
	{
		if(hWnd == NULL)
		{
			LUARETURN_NIL(l, 0);
		}

		MSG msg;
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				LUARETURN_NIL(l, 0);
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		LUARETURN(l, 1);
	}

	int ShowFps(lua_State *l)
	{
		static DWORD    last     = timeGetTime();
		static DWORD    frames   = 0;
		static char     buf[256] = "";
		DWORD           current;
		current = timeGetTime();
		frames++;
		if(1000 <= current - last)
		{
			float dt  = (float)(current - last) / 1000.0f;
			float fps = (float)frames / dt;
			last      = current;
			frames    = 0;
			sprintf(buf, "%.02f fps", fps);
			printf("%s\n", buf);
		}
		LUARETURN(l, 1);
	}
}

namespace luawindow
{
	void Init(lua_State *l)
	{
		LUAPUSHGFN(l, RunApp);
		LUAPUSHGFN(l, ProcMsg);
		LUAPUSHGFN(l, ShowFps);
	}
	
	HWND GetHwnd()
	{
		if(hWnd == NULL)
		{
			printf("Warning HWND is nil\n");
		}
		return hWnd;
	}
}

