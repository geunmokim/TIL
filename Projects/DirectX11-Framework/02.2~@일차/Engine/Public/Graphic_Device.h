#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

public:
	typedef struct tagGraphicDeviceDesc
	{
		HWND			hWnd;
		unsigned int	iWinCX, iWinCY;
		WINMODE			eWinMode;
	}GRAPHICDEVDESC;

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:

	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE WinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	//이중 포인터로 매개변수 받는 이유 -> 호출한 쪽의 포인터 변수 자체를 채워주기 위해서 
	//정확히 말하면 Client쪽에 있는 pDevice 라는 변수 자체에 방금 만든 m_pDevice 주소를 채워넣기 위해
	//포인터형의 값을 받아와서 수정하기 위해 이중포인터 사용 

	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

private:

	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) */
	ID3D11Device* m_pDevice = nullptr;

	/*메모리 할당. (정점 버퍼 , 인덱스 버퍼 , 텍스쳐 로드 */
	/* 바인딩작업. 기능실행 (정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	//화면에 보여줄 프레임 버퍼를 앞/뒤로 교체 (더블 버퍼링)해주는 역할
	IDXGISwapChain* m_pSwapChain = nullptr;

	// ID3D11ShaderResourceView*
	//여기에 그림을 그려라 라고 지정하는 대상
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;

	//깊이(어떤 물체가 앞에 있는지) 테스트용 버퍼 
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

	/*
	* 실제 DX11 초기화 순서 
	  Ready_SwapChain (디바이스+스왑체인 생성)
   → Ready_BackBufferRenderTargetView (스왑체인의 백버퍼로 RTV 생성)
   → Ready_DepthStencilRenderTargetView (깊이버퍼 별도 생성)
	
	*/

public:
	virtual void Free() override;
};

END