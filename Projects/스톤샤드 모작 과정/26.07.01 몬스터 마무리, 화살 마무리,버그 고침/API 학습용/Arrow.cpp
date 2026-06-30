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

	m_iLifeFrame = 8;	// 이번 프레임은 살아서 Render까지 호출되고, 다음 프레임에 죽는다.

	m_iOffsetX = 0;	// Set_Direction이 안 불려도 안전하게 칸 중앙 기본값
	m_iOffsetY = 0;

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


	// m_tRect.left/top은 칸(TILECX/TILECY=64) 기준 좌상단이라,
	// 21x21 스프라이트를 그대로 그리면 칸 왼쪽으로 쏠려 보임.
	// 칸 중앙에 오도록 (64-21)/2 만큼 보정.

	int iOffsetX = (TILECX - 21) / 2;
	int iOffsetY = (TILECY - 21) / 2;

	GdiTransparentBlt(_DC,
		m_tRect.left + iScrollX + m_iOffsetX, m_tRect.top + iScrollY + m_iOffsetY,
		21, 21,
		hArrowDC,
		0, 0,
		21, 21,
		RGB(255, 255, 255));
}
void CArrow::Release()
{

}
