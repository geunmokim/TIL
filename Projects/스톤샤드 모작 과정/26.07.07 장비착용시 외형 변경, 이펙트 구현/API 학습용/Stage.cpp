#include "framework.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "NewPlayer.h"
#include "Sight.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Monster_Melee.h"
#include "Monster_Range.h"
#include "TurnMgr.h"
#include "Inven.h"


CStage::CStage()
{

}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Tile.bmp"), _T("Tile"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/UI/hp_bg_red.bmp"), _T("HP_BG"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/UI/hp_fill_red.bmp"), _T("HP_FILL"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/cleaver_1.bmp"), _T("Cleaver_Cursor"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/cleaver_0.bmp"), _T("Char_Cleaver"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/gambeson_1.bmp"), _T("Char_Gambeson"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/hat_1.bmp"), _T("Char_Hat"));

	CTileMgr::Get_Instance()->Load_Tile();
	CSight::Get_Instance()->Initialize();
	CTurnMgr::Get_Instance()->Initialize();
	CInven::Get_Instance()->Initialize();
	CObj* pPlayer = CAbFactory<CNewPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pPlayer);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pPlayer);

	CObj* pMonster_Melee = CAbFactory<CMonster_Melee>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Melee);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Melee);

	CObj* pMonster_Range = CAbFactory<CMonster_Range>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Range);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Range);

	



	CSight::Get_Instance()->Update_Sight();
}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
	CTurnMgr::Get_Instance()->Update();
	CInven::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}
void CStage::Render(HDC _DC)
{
	//마우스 커서 숨기기용
	static bool bPrevCursor = true;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	CTileMgr::Get_Instance()->Render(_DC);
	CSight::Get_Instance()->Render(_DC);
	
	CObjMgr::Get_Instance()->Render(_DC);
	CInven::Get_Instance()->Render(_DC);  //CObjMgr보다 먼저 호출하면 플레이어가 인벤토리 위에 그려져서 부자연스러워진다


	if (g_bCursorOnMonster)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		
		// 커서 숨기기 (상태 바뀔때만)
		if (bPrevCursor)
		{
			ShowCursor(FALSE);
			bPrevCursor = false;
		}

		HDC hCursorDC = CBmpMgr::Get_Instance()->Find_Image(_T("Cleaver_Cursor"));
		GdiTransparentBlt(_DC, pt.x - 24, pt.y - 20, 48, 40,
			hCursorDC, 0, 0, 48, 40, RGB(255, 255, 255));


	}
	else if (!g_bCursorOnMonster && !bPrevCursor)
	{
		ShowCursor(TRUE);
		bPrevCursor = true;
	}




	//hp바 
	CObj* pPlayerHp = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayerHp)
	{


		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayerHp;

		HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(L"HP_BG");
		HDC hFillDC = CBmpMgr::Get_Instance()->Find_Image(L"HP_FILL");

		int iX = 10;
		int iY = 30;

		// 배경 전체
		BitBlt(_DC, iX, iY, 167, 8, hBgDC, 0, 0, SRCCOPY);

		// HP 비율만큼 채움
		int iHpWidth = (int)(167.f * ((float)pNewPlayer->Get_Hp() / pNewPlayer->Get_MaxHp()));
		if (iHpWidth > 0)
			BitBlt(_DC, iX, iY, iHpWidth, 8, hFillDC, 0, 0, SRCCOPY);
	}


	

	//플레이어 스탯을 텍스트로 표현
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
		TCHAR sz[64];
		wsprintf(sz, _T("HP:%d ATK:%d DEF:%d EXP:%d LV:%d"),
			pNewPlayer->Get_Hp(),
			pNewPlayer->Get_Atk(),
			pNewPlayer->Get_Def(),
			pNewPlayer->Get_Exp(),
			pNewPlayer->Get_Level());
		SetBkMode(_DC, TRANSPARENT);
		SetTextColor(_DC, RGB(255, 255, 0));
		TextOut(_DC, 10, 10, sz, lstrlen(sz));
	}

}
void CStage::Release()
{

}