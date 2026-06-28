#include "framework.h"
#include "Arrow.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
CArrow::CArrow()
{

}

CArrow::~CArrow()
{
	Release();
}

void CArrow::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Arrow/s_curvedbroadheadarrow_shoot.bmp"), _T("Arrow"));

	m_tInfo.iCX = TILECX;   
	m_tInfo.iCY = TILECY;

	m_pImageKey = _T("Arrow");
	m_eRenderID = RENDERID::EFFECT;   
	m_bDead = false;
	m_iLifeFrame = 1;	// 이번 프레임은 살아서 Render까지 호출되고, 다음 프레임에 죽는다.
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

	GdiTransparentBlt(_DC,
		m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hArrowDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
		RGB(255, 255, 255));

}
void CArrow::Release()
{

}
