#include "framework.h"
#include "NewMonster.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CNewMonster::CNewMonster()
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

	GdiTransparentBlt(_DC,
		m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hMonsterDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
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