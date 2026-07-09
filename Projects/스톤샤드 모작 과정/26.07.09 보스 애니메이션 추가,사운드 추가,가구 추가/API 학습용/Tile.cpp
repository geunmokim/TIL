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

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC hTileDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile");

	// BitBlt -> StretchBlt로 변경: 원본(26x26)을 목적지 크기(64x64)로 확대해서 그림
	StretchBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY   // 목적지 좌상단 (+Y스크롤도 추가)
		, m_tInfo.iCX, m_tInfo.iCY                            // 목적지 크기 (64x64로 확대)
		, hTileDC
		, m_iDrawID * 26, 0                                    // 원본 시작 위치 (26px 간격)
		, 26, 26                                                // 원본에서 가져올 크기 (26x26)
		, SRCCOPY);
}

void CTile::Release()
{

}
