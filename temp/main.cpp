
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


namespace Dx12 {
	
	/////////////////////////////////////////////////////////////
	//
	// CommandQueue
	//
	/////////////////////////////////////////////////////////////
	class CommandQueue {
		D3D12_COMMAND_QUEUE_DESC desc = {};
		ID3D12CommandQueue       *queue;
	public:
		CommandQueue(ID3D12Device *device) {
			D3D12_COMMAND_QUEUE_DESC temp = {};
			desc = temp;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
			device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
		}
		~CommandQueue() { RELEASE(queue); }
		ID3D12CommandQueue *Get() { return queue; }
	};

	/////////////////////////////////////////////////////////////
	//
	// ID3D12CommandList
	//
	/////////////////////////////////////////////////////////////
	class ID3D12CommandAllocatorList {
		D3D12_COMMAND_QUEUE_DESC  desc;
		ID3D12CommandAllocator    *allocator;
		ID3D12GraphicsCommandList *m_commandList;
		ID3D12GraphicsCommandList *m_bundle;
	public:
		ID3D12CommandAllocatorList(ID3D12Device *device, D3D12_COMMAND_LIST_TYPE  type) {
			D3D12_COMMAND_QUEUE_DESC temp = {};
			desc = temp;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Type  = type;
			device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
			device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_bundleAllocator));
		}
		~ID3D12CommandAllocatorList() {
			RELEASE(allocator);
			RELEASE(commandList);
			RELEASE(bundle);
		}
	};

	/////////////////////////////////////////////////////////////
	//
	// DescriptorHeap
	//
	/////////////////////////////////////////////////////////////
	class DescriptorHeap {
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		ID3D12DescriptorHeap      *heap;
		DWORD                      count;
	public:
		DescriptorHeap(ID3D12Device *device, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAG flag, typeint count) {
			D3D12_DESCRIPTOR_HEAP_DESC temp = {};
			desc = temp;
			desc.NumDescriptors = FrameCount;
			desc.Type           = type;
			desc.Flags          = flag;
			device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
			count = FrameCount;
		}
		~DescriptorHeap()
		{
			memset(&desc, 0, sizeof(desc));
			heap = NULL;
			count = 0;
		}
		ID3D12DescriptorHeap *Get()
		{
			return heap;
		}
	};

	/////////////////////////////////////////////////////////////
	//
	// RootSignature
	//
	/////////////////////////////////////////////////////////////
	class RootSignature {
		ID3D12RootSignature *rootSignature;
	public:
		RootSignature() {
			int count = 1;
			D3D12_ROOT_DESCRIPTOR      rooDescripter = {}
			rooDescripter.ShaderRegister;
			rooDescripter.RegisterSpace;
			D3D12_DESCRIPTOR_RANGE     ranges[count]         = {};
			D3D12_ROOT_PARAMETER       rootParameters[count] = {};
			D3D12_ROOT_SIGNATURE_DESC  rootSignatureDesc     = {};

			ranges[0].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			ranges[0].registerSpace      = 1
			ranges[0].BaseShaderRegister = 0
			ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			rootParameters[0].InitAsDescriptorTable(count, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			// Allow input layout and deny uneccessary access to certain pipeline stages.
			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			rootSignatureDesc.Initcount, rootParameters, NULL, nullptr, rootSignatureFlags); //need sampler

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ThrowIfFailed(D3D12SerializeRootSignature(
				&rootSignatureDesc, 
				D3D_ROOT_SIGNATURE_VERSION_1, 
				&signature, 
				&error));
			ThrowIfFailed(
				m_device->CreateRootSignature(
					0, signature->GetBufferPointer(),
					signature->GetBufferSize(),
					IID_PPV_ARGS(&m_rootSignature)));

			ID3DBlob signature;
		}
	};

	/////////////////////////////////////////////////////////////
	//
	// SwapChain
	//
	/////////////////////////////////////////////////////////////
	class SwapChain {
		DXGI_SWAP_CHAIN_DESC  desc;
		IDXGISwapChain3      *swapchain;
	public:
		SwapChain(Factory *factory, CommandQueue *queue, HWND hWnd, int widthm int height, int FrameCount, int &FrameIndex) {
			DXGI_SWAP_CHAIN_DESC temp = {};
			desc = temp;
			IDXGISwapChain *tempref = NULL;;
			desc.BufferCount       = FrameCount;
			desc.BufferDesc.Width  = width;
			desc.BufferDesc.Height = height;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.OutputWindow      = hWnd;
			desc.SampleDesc.Count  = 1;
			desc.Windowed          = TRUE;
			factory->CreateSwapChain(queue->Get(), &desc, &tempref);
			tempref->QueryInterface( IID_PPV_ARGS( &swapchain ) );
			FrameIndex = swapchain->GetCurrentBackBufferIndex();
			RELEASE(tempref);
		}
		~SwapChain()
		{
			memset(&desc, 0, sizeof(desc));
			RELEASE(swapchain);
		}
		IDXGISwapChain3 *Get() {
			return swapchain;
		}
	};
	
	



	/////////////////////////////////////////////////////////////
	//
	// RenderTarget
	//
	/////////////////////////////////////////////////////////////
	class ID3D12Resource {
		ID3D12Resource *resource;
		ID3D12Resource() {
			resource = NULL;
		}
		~ID3D12Resource()
		{
			RELEASE();
		}
		ID3D12Resource *Set(ID3D12Resource *resource)
		{
		}
		ID3D12Resource *Get()
		{
			return resource;
		}
		
	};

#define DEBUG(x) printf("(%08d)%s : %08X\n", __LINE__, __FUNCTION__, x)

	//Mas

	class Device {
		//‚í‚¯‚éˆÓ–¡‚ª‚È‚¢
		IDXGIFactory4            *factory;
		IDXGIAdapter1            *hardwareAdapter;
		ID3D12Device             *device;
		enum {
			FrameCount = 2,
		};
	public:
		Device(HWND h, DWORD width, DWORD height) {
			CreateDXGIFactory1(IID_PPV_ARGS(&factory));
			D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)); // D3D_FEATURE_LEVEL_12_1
			
			CommandQueue queue(device);
			int frameIndex = 999999;
			SwapChain swapchain(factory, &queue, hWnd, Width, Height, FrameCount, frameIndex);
			factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
		}
		
		ID3D12Device *Get()
		{
			return device;
		}

		DWORD GetDescIncSize(D3D12_DESCRIPTOR_HEAP_TYPE e)
		{
			return device->GetDescriptorHandleIncrementSize(e);
		}
		
		ID3D12DescriptorHeap *CreaetDescHeap(D3D12_DESCRIPTOR_HEAP_TYPE type,  D3D12_DESCRIPTOR_HEAP_FLAG flags ) {
			ID3D12DescriptorHeap *ret = NULL;
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.NumDescriptors = 1; //todo
			desc.Type           = type;
			desc.Flags          = flags;
			m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&ret));
		}
		
	};

	/////////////////////////////////////////////////////////////
	//
	// SwapRenderTargetView
	//
	/////////////////////////////////////////////////////////////	
	class SwapRenderTargetView {
		enum
		{
			MaxCount = 2
		};
		ID3D12DescriptorHeap      *heap;
		ID3D12Resource             renderTargets[MaxCount];
		/*
		D3D12_VIEWPORT m_viewport;
		ComPtr<IDXGISwapChain3> m_swapChain;
		ComPtr<ID3D12Device> m_device;
		ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
		ComPtr<ID3D12Resource> m_depthStencil;
		ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12RootSignature >m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
		ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
		ComPtr<ID3D12DescriptorHeap> m_samplerHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState1;
		ComPtr<ID3D12PipelineState> m_pipelineState2;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		D3D12_RECT m_scissorRect;*/

	public:
		~SwapRenderTargetView()
		{
			for(UINT i = 0 ; i < MaxCount; i++)
			{
				RELEASE(renderTargets[i]);
			}
			RELEASE(heap);
		}
		SwapRenderTargetView(Device *device, SwapChain *swapchain, int FrameCount)
		{
			if(FrameCount != MaxCount)
			{
				printf("‚É‚±‚µ‚©‚³‚Û[‚Æ‚µ‚Ä‚¢‚Ü‚¹‚ñB‚µ‚ËI");
				return;
			}
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			device->Get()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&heap));

			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = heap->GetCPUDescriptorHandleForHeapStart();
		
			SIZE_T  rtvDescriptorSize    = device->GetDescIncSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			SIZE_T  cbvSrvDescriptorSize = device->GetDescIncSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			for (UINT i = 0; i < FrameCount; i++) {
				swapchain->Get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
				device->Get()->CreateRenderTargetView(renderTargets[i], NULL, rtvHandle);
				rtvHandle.ptr += rtvDescriptorSize
			}
		}
	};
	class App{
	public:
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
			
		}
	};
}

int main()
{
	WinApp app("TEST", WIN_X, WIN_Y);
	Dx12 dx12;
	dx12.Init(app.GetHwnd(), app.GetWidth(), app.GetHeight());
	while(app.ProcMsg()) {
	}
	return 0;
}

