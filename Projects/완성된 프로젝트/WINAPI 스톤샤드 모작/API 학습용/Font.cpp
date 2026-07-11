#include "framework.h"
#include "Font.h"
#include "ScrollMgr.h"

CFont::CFont()
{

}
CFont::CFont(int _iDamage, const GRIDPOS& _targetPos)
	:m_iDamage(_iDamage),m_iLifeFrame(30),m_tTargetPos(_targetPos)
{
	
}
CFont::~CFont()
{
	Release();
}

void CFont::Initialize()
{
	Set_GridPos(m_tTargetPos.row, m_tTargetPos.col);
}

int CFont::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_iLifeFrame <= 0)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}
	--m_iLifeFrame;

	return OBJ_NOEVENT;
}

void CFont::Late_Update()
{

}
void CFont::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	// °ÝÀÚ Ä­ ¡æ ÇÈ¼¿ ÁÂÇ¥ º¯È¯
	int iPixelX = (int)(m_tInfo.fX + iScrollX);
	int iPixelY = (int)(m_tInfo.fY + iScrollY);

	TCHAR szDamage[16];
	wsprintf(szDamage, _T("-%d"), m_iDamage);

	SetBkMode(_DC, TRANSPARENT);
	SetTextColor(_DC, RGB(255, 50, 50));  // »¡°£»ö
	TextOut(_DC, iPixelX, iPixelY, szDamage, lstrlen(szDamage));
}


void CFont::Release()
{

}