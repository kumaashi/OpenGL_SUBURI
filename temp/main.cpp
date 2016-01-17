
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <windows.h>
#include <windowsx.h>


#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")



class File {
	std::vector<unsigned char> vBuf;
public:
	int Size()  { return vBuf.size(); }
	void *Buf() { return &vBuf[0]; }
	File(const char *name) {
		Open(name);
	}
	~File() {
		Close();
	}
	void Close() {}
	int Open(const char *name) {
		vBuf.clear();
		FILE *fp = fopen(name, "rb");
		if(fp) {
			fseek(fp, 0, SEEK_END);
			int Size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			vBuf.resize(Size);
			memset(&vBuf[0], 0, vBuf.size());
			fread(&vBuf[0], 1, Size, fp);
			fclose(fp);
		}
		return Size();
	}
};




class WinApp {
	const char *AppName;
	BOOL Windowed  ;
	HWND hWnd;
	static int  Width ;
	static int  Height;
	static int  xPos  ;
	static int  yPos  ;
	static BOOL mLeft ;
	static BOOL mRight;
	
	static LRESULT CALLBACK WinApp::MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	WinApp(const char *name, int width, int height)
	{
		AppName        = "dummy";
		Width          = 0;
		Height         = 0;
		hWnd           = NULL;
		Windowed       = TRUE;
		xPos           = 0;
		yPos           = 0;
		mLeft          = FALSE;
		mRight         = FALSE;
		Run(name, width, height);
	}
	
	
	int GetWidth()
	{
		return Width;
	}

	int GetHeight()
	{
		return Height;
	}
	
	

	~WinApp()
	
	{
		Term();
	}
	
	void Term()
	{
	}

	int Run(const char *name, int width, int height)
	{
		AppName = name;
		Width = width;
		Height = height;

		printf("%s : %s -> %d %d : ", __FUNCTION__, AppName, Width, Height);

		static WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC | CS_VREDRAW | CS_HREDRAW,
			WinApp::MainWindowProc, 0L, 0L, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
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
			return 0;
		}

		hWnd = CreateWindowEx(
				   styleex, AppName, AppName, style,
				   (GetSystemMetrics(SM_CXSCREEN) - rc.right)  / 2,
				   (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
				   rc.right, rc.bottom, 0, 0, wcex.hInstance, (void *)this);

		if(!hWnd)
		{
			printf("%s : failed CreateWindowEx\n", __FUNCTION__);
			return 0;
		}

		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		printf(" Done.\n");

		return 0;
	}

	int ProcMsg()
	{
		if(hWnd == NULL)
		{
			return 0;
		}

		MSG msg;
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				return 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return 1;
	}

	int ShowFps()
	{
		static DWORD	last	 = timeGetTime();
		static DWORD	frames   = 0;
		static char	 buf[256] = "";

		DWORD		   current;
		current = timeGetTime();
		frames++;

		if(1000 <= current - last)
		{
			float dt  = (float)(current - last) / 1000.0f;
			float fps = (float)frames / dt;
			last	  = current;
			frames	= 0;
			sprintf(buf, "%.02f fps", fps);
			printf("%s\n", buf);
		}

		return 0;
	}
	
	
	HWND GetHwnd()
	{
		if(hWnd == NULL)
		{
			printf("Warning HWND is nil\n");
		}
		return hWnd;
	}
};


int  WinApp::Width  = 0;
int  WinApp::Height = 0;
int  WinApp::xPos   = 0;
int  WinApp::yPos   = 0;
BOOL WinApp::mLeft  = 0;
BOOL WinApp::mRight = 0;
	
LRESULT CALLBACK WinApp::MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int temp = wParam & 0xFFF0;
	switch(msg)
	{
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


#define RELEASE(x) {if(x) {x->Release(); x = NULL; } } 

#define WIN_X 800
#define WIN_Y 600



#include <d3d12.h>
#include <dxgi1_4.h>


#define FrameCount 2

class Dx12 {
public:
	IDXGIFactory4            *factory;
	IDXGIAdapter1            *hardwareAdapter;
	ID3D12Device             *m_device;
	IDXGISwapChain           *m_swapChainRef;
	IDXGISwapChain3          *m_swapChain;
	ID3D12CommandQueue       *m_commandQueue;
	ID3D12DescriptorHeap     *m_rtvHeap;
	ID3D12Resource           *m_renderTargets[2];
	DWORD                     m_frameIndex;
	DWORD                     m_rtvDescriptorSize;
	Dx12() {
		factory = NULL;
		hardwareAdapter = NULL;
		m_device = NULL;
		m_swapChain = NULL;
		m_commandQueue = NULL;
		m_frameIndex = 0;
	}

	void Term() {
		RELEASE(m_swapChain);
		RELEASE(m_device);
		RELEASE(hardwareAdapter);
		RELEASE(factory);
	}
	void Init(HWND hWnd, int width, int height) {
		CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		printf("Device -> %08X\n", factory);
		D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device));
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

		m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount       = FrameCount;
		swapChainDesc.BufferDesc.Width  = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.OutputWindow      = hWnd;
		swapChainDesc.SampleDesc.Count  = 1;
		swapChainDesc.Windowed          = TRUE;

		factory->CreateSwapChain(m_commandQueue, &swapChainDesc, &m_swapChainRef);
		m_swapChainRef->QueryInterface( IID_PPV_ARGS( &m_swapChain ) );
		factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		for (UINT n = 0; n < FrameCount; n++)
		{
			m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_device->CreateRenderTargetView(m_renderTargets[n], NULL, rtvHandle);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
	}
};

int main()
{
	WinApp app("TEST", WIN_X, WIN_Y);
	Dx12 dx12;
	dx12.Init(app.GetHwnd(), app.GetWidth(), app.GetHeight());
	while(app.ProcMsg()) {
	}
	return 0;
}

