#include "framework.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CTile::CTile()
{

}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

}

int CTile::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;

}

void CTile::Late_Update()
{

}
void CTile::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	HDC hTileDC = CBmpMgr::Get_Instance()->Find_Image(_T("Tile"));

	BitBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hTileDC
		, m_iDrawID * TILECX, 0
		, SRCCOPY);
}

void CTile::Release()
{

}
