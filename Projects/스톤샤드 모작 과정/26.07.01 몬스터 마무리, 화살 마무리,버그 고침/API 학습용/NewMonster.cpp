#include "framework.h"
#include "NewMonster.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"

CNewMonster::CNewMonster()
	:m_iSrcCX(0), m_iSrcCY(0),m_iAtk(0)
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

	GdiTransparentBlt(_DC,
		m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		m_iSrcCX, m_iSrcCY,
		hMonsterDC,
		0, 0,
		m_iSrcCX, m_iSrcCY,
		RGB(255, 255, 255));

}
void CNewMonster::Release()
{

}
void CNewMonster::Chase(const GRIDPOS& _next)
{
	//PathFind가 알려준 다음 칸이 다른 오브젝트로 점유되어 있으면 이동하지 않는다
	if (CObjMgr::Get_Instance()->Is_Occupied(_next))
		return;

	//이동시 좌표를 다음칸으로
	Set_GridPos(_next.row, _next.col);
}