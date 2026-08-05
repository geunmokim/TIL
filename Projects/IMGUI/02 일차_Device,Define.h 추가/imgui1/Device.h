#pragma once
#include <d3d11.h>
#include "Define.h"

// 이 창에 그림 그릴 수 있게 그래픽 카드 연결해줘 담당 클래스
class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	HRESULT Init_Device(HWND hWnd);

	void Render_Begin(const float* pClearColor); //매 프레임 화면 지우기
	void Render_End(void);    //화면에 띄우기(Present)

	void Release(void);
private:
	ID3D11Device* m_pDevice; //그래픽 자원을 만드는 역할
	ID3D11DeviceContext* m_pContext;  //실제로 그려라 명령을 내리는 역할
	IDXGISwapChain* m_pSwapChain; //화면 두 장(보이는 것 / 미리 그리는 것) 관리
	ID3D11RenderTargetView* m_pRenderTargetView; //여기가 그릴 대상이다 표시

};

