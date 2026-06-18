#include "framework.h"
#include "Mouse.h"
CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 100;
}

int CMouse::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	POINT	pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	//ShowCursor(false);

	m_tInfo.fX = (float)pt.x;
	m_tInfo.fY = (float)pt.y;


	Update_Rect();
	return OBJ_NOEVENT;
	//winapi 마우스 처리 함수들인데 
	/*
	 GetCursorPos  → 마우스 현재 위치 가져오기
     ScreenToClient → 화면 좌표를 윈도우 기준으로 변환
     ShowCursor    → 커서 보이기/숨기기
	정도만 알아두기 어차피 나중가면 쓸일 없다한다
	*/
}

void CMouse::Late_Update()
{

}

void CMouse::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);  //마우스의 모양(랜더)
}

void CMouse::Release()
{

}