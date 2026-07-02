#include "framework.h"
#include "NewMonster.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CNewMonster::CNewMonster()
	:m_iSrcCX(0), m_iSrcCY(0),m_iAtk(0),m_bFlipX(false)
{

}

CNewMonster::~CNewMonster()
{
	Release();
}

int CNewMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;

}

void CNewMonster::Late_Update()
{

}
void CNewMonster::Render(HDC _DC)
{
	Update_Rect();
	
	HDC hMonsterDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iDestX = m_tRect.left + iScrollX;
	int iDestY = m_tRect.top + iScrollY;


	if (m_bFlipX)
	{
		HDC hTempDC = CreateCompatibleDC(_DC);
		HBITMAP hTempBmp = CreateCompatibleBitmap(_DC, m_iSrcCX, m_iSrcCY);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hTempBmp);

		RECT rc = { 0, 0, m_iSrcCX, m_iSrcCY };
		FillRect(hTempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		StretchBlt(hTempDC, m_iSrcCX - 1, 0, -m_iSrcCX, m_iSrcCY,
			hMonsterDC, 0, 0, m_iSrcCX, m_iSrcCY, SRCCOPY);

		GdiTransparentBlt(_DC,
			iDestX, iDestY, m_iSrcCX, m_iSrcCY,
			hTempDC, 0, 0, m_iSrcCX, m_iSrcCY,
			RGB(255, 255, 255));

		SelectObject(hTempDC, hOldBmp);
		DeleteObject(hTempBmp);
		DeleteDC(hTempDC);
	}
	else
	{
		GdiTransparentBlt(_DC,
			iDestX, iDestY, m_iSrcCX, m_iSrcCY,
			hMonsterDC, 0, 0, m_iSrcCX, m_iSrcCY,
			RGB(255, 255, 255));
	}

}
void CNewMonster::Release()
{

}
void CNewMonster::Chase(const GRIDPOS& _next)
{
	//PathFind가 알려준 다음 칸이 다른 오브젝트로 점유되어 있으면 이동하지 않는다
	if (CObjMgr::Get_Instance()->Is_Occupied(_next))
		return;
	//다음 칸의 col이 현재보다 크면 오른쪽 이동 -> 반전
	if (_next.col > m_tGridPos.col)
		m_bFlipX = true;
	else if (_next.col < m_tGridPos.col)
		m_bFlipX = false;
	//col이 같으면 (상하 이동) 현재 방향 유지

	//이동시 좌표를 다음칸으로
	Set_GridPos(_next.row, _next.col);
}