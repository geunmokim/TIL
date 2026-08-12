#include <windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Device.h"
IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRenderTargetView(nullptr)
{
}

CDevice::~CDevice()
{
}

HRESULT CDevice::Init_Device(HWND hWnd)
{
	// "이런 식으로 그려줘" 설정값 뭉치
	DXGI_SWAP_CHAIN_DESC	SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;								// 더블 버퍼링
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = hWnd;							//Day1에서 만든 창
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL		FeatureLevel;
	const D3D_FEATURE_LEVEL	FeatureLevelArr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

	// 그래픽카드 드라이버한테 "DX11로 그릴 준비해줘" 요청
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		FeatureLevelArr, 2, D3D11_SDK_VERSION,
		&SwapChainDesc, &m_pSwapChain, &m_pDevice, &FeatureLevel, &m_pContext);

	if (FAILED(hr))
	{
		ERR_MSG(L"D3D11CreateDeviceAndSwapChain Failed");
		return E_FAIL;
	}

	// 방금 만든 스왑체인의 뒷면 버퍼를 꺼내서, 거기 그릴 수 있게 뷰(View)를 만듦
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();

	return S_OK;
}

void CDevice::Render_Begin(const float* pClearColor)
{
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);	// 그릴 대상 설정
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, pClearColor);	// 지정한 색으로 화면 지우기
}

void CDevice::Render_End(void)
{
	m_pSwapChain->Present(1, 0);	// 미리 그려둔 화면을 실제로 보이게 전환 (VSync 켬)
}

void CDevice::Release(void)
{
	if (m_pRenderTargetView) { m_pRenderTargetView->Release();	m_pRenderTargetView = nullptr; }
	if (m_pSwapChain) { m_pSwapChain->Release();			m_pSwapChain = nullptr; }
	if (m_pContext) { m_pContext->Release();			m_pContext = nullptr; }
	if (m_pDevice) { m_pDevice->Release();				m_pDevice = nullptr; }
}

/*
m_pDevice	그래픽 자원을 만드는 공장	텍스처, 버퍼 새로 만들 때
m_pContext	실제로 그리라고 명령 내림	매 프레임 그리기 코드 짤 때
m_pSwapChain	화면 두 장 관리(더블버퍼링)	창 크기 바뀔 때(리사이즈)
m_pRenderTargetView	"여기가 그릴 대상" 표시	렌더타겟 여러 개 쓸 때(그림자맵 등)

나머지는 이러한것이 있다 정도만 알면된다 
*/