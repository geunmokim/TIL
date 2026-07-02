#include "framework.h"
#include "Arrow.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CArrow::CArrow()
{

}

CArrow::~CArrow()
{
	Release();
}

void CArrow::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/s_curvedbroadheadarrow_shoot.bmp"), _T("Arrow"));

	m_tInfo.iCX = TILECX;   
	m_tInfo.iCY = TILECY;

	m_pImageKey = _T("Arrow");
	m_eRenderID = RENDERID::EFFECT;   
	m_bDead = false;

	m_iLifeFrame = 60;	// 이번 프레임은 살아서 Render까지 호출되고, 다음 프레임에 죽는다.

	m_iOffsetX = 0;	// Set_Direction이 안 불려도 안전하게 칸 중앙 기본값
	m_iOffsetY = 0;


	m_bFlipX = false;
}

int CArrow::Update()
{
	if(m_bDead)
	return OBJ_DEAD;

	Update_Rect();

	if (m_iLifeFrame <= 0)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}
	--m_iLifeFrame;

	return OBJ_NOEVENT;


}
void CArrow::Late_Update()
{

}
void CArrow::Render(HDC _DC)
{
	Update_Rect();


	HDC hArrowDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iDestX = m_tRect.left + m_iOffsetX + iScrollX;
	int iDestY = m_tRect.top + m_iOffsetY + iScrollY;
	if (m_bFlipX)
	{
		// GdiTransparentBlt는 음수 폭을 지원하지 않아 반전이 안 됨.
		// StretchBlt는 음수 폭으로 반전이 가능하므로,
		// 임시 DC에 반전 복사 후 TransparentBlt로 그린다.
		HDC hTempDC = CreateCompatibleDC(_DC);
		HBITMAP hTempBmp = CreateCompatibleBitmap(_DC, 21, 21);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hTempBmp);

		// 투명색(흰색)으로 배경 초기화
		RECT rc = { 0, 0, 21, 21 };
		FillRect(hTempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// x 시작점을 오른쪽 끝(20)으로, 폭을 -21로 주면 좌우 반전
		StretchBlt(hTempDC, 20, 0, -21, 21,
			hArrowDC, 0, 0, 21, 21, SRCCOPY);

		GdiTransparentBlt(_DC,
			iDestX, iDestY, 21, 21,
			hTempDC, 0, 0, 21, 21,
			RGB(255, 255, 255));

		SelectObject(hTempDC, hOldBmp);
		DeleteObject(hTempBmp);
		DeleteDC(hTempDC);
	}
	else
	{
		GdiTransparentBlt(_DC,
			iDestX, iDestY, 21, 21,
			hArrowDC, 0, 0, 21, 21,
			RGB(255, 255, 255));
	}
}
void CArrow::Release()
{

}
