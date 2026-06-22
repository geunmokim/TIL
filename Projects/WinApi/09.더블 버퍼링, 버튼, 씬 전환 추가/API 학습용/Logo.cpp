#include "framework.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CLogo::CLogo()
{

}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Logo/Logo.bmp"), _T("Logo"));

}

void CLogo::Update()
{

}

void CLogo::Late_Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)) //VK_RETURN은 ENTER키를 의미한다
	{
		CSceneMgr::Get_Instance()->Scene_Chaange(CSceneMgr::MENU);
		return;

	}
}

void CLogo::Render(HDC _DC)
{
	HDC hLogoDC = CBmpMgr::Get_Instance()->Find_Image(_T("Logo"));
	BitBlt(_DC, 0, 0, WINCX, WINCY, hLogoDC, 0, 0, SRCCOPY);  //BitBlt는 GDI(Windows의 2D 그래픽 API)에서 이미지를 화면에 복사해서 그리는 함수
	/*
	BOOL BitBlt(
    HDC hdcDest,    // 그려질 대상 (도화지)
    int x, int y,   // 대상에서 그릴 시작 위치
    int cx, int cy,  // 복사할 너비, 높이
    HDC hdcSrc,     // 원본 이미지가 있는 곳
    int x1, int y1,  // 원본에서 가져올 시작 위치
    DWORD rop        // 복사 방식 (보통 SRCCOPY)
	);
	*/
	
	
}

void CLogo::Release()
{

}