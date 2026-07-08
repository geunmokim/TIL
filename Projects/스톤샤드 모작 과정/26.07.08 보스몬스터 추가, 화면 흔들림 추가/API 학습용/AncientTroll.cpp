#include "framework.h"
#include "AncientTroll.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Sight.h"

CAncientTroll::CAncientTroll()
{

}
CAncientTroll::~CAncientTroll()
{

}

void CAncientTroll::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ancienttroll_sheet.bmp"), _T("AncientTroll"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	m_iSrcCX = 101;  //스프라이트 시트 1프레임당 크기
	m_iSrcCY = 105;  //스프라이트 시트 1프레임당 크기

	
	m_tFrame.iStartX = 0;
	m_tFrame.iEndX = 20;
	m_tFrame.iStateY = 0;
	m_tFrame.dwDelayTime = 120;
	m_tFrame.dwTime = GetTickCount();

	Set_GridPos(30, 40); // 하단-우측 보스룸, 시작 칸 위치 //일단 임의로 잡아둔 값
	m_iHp = 250;
	m_iAtk = 30;
	m_iExp = 500;

	m_pImageKey = _T("AncientTroll");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
}

int CAncientTroll::Update()
{
	int iEvent = CMonster_Melee::Update();   //죽음 체크 + Update_Rect()는 CNewMonster::Update()를 그대로 재사용

	if (OBJ_DEAD == iEvent)
		return OBJ_DEAD;

	Update_Frame();   //CObj의 프레임 반복재생 로직 재사용 -> idle 애니메이션

	return OBJ_NOEVENT;
}

void CAncientTroll::Render(HDC _DC)
{

	if (CSight::Get_Instance()->Get_Visibility(m_tGridPos.row, m_tGridPos.col) != VISIBLE)
		return;

	Update_Rect();

	HDC hMonsterDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollY();

	int iDestX = m_tRect.left + iScrollX;
	int iDestY = m_tRect.top + iScrollY;

	//스프라이트 시트에서 현재 프레임 위치만큼 소스 X를 이동해서 잘라옴
	int iSrcX = m_tFrame.iStartX * m_iSrcCX;

	if (m_bFlipX)
	{
		HDC hTempDC = CreateCompatibleDC(_DC);
		HBITMAP hTempBmp = CreateCompatibleBitmap(_DC, m_iSrcCX, m_iSrcCY);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hTempBmp);

		RECT rc = { 0, 0, m_iSrcCX, m_iSrcCY };
		FillRect(hTempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		StretchBlt(hTempDC, m_iSrcCX - 1, 0, -m_iSrcCX, m_iSrcCY,
			hMonsterDC, iSrcX, 0, m_iSrcCX, m_iSrcCY, SRCCOPY);

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
			hMonsterDC, iSrcX, 0, m_iSrcCX, m_iSrcCY,
			RGB(255, 255, 255));
	}
}

void CAncientTroll::Attack(CObj* _pTarget)
{
	
	CMonster_Melee::Attack(_pTarget);
	CScrollMgr::Get_Instance()->Start_Shake(6.f, 200);

}

void CAncientTroll::Take_Damage(int _dmg)
{
	CNewMonster::Take_Damage(_dmg);
	CScrollMgr::Get_Instance()->Start_Shake(4.f, 150);   //때릴 때 약하게
}

