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
	
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Tile.bmp", L"Tile");
	
	
	
	CTileMgr::Get_Instance()->Load_Tile();
	CSight::Get_Instance()->Initialize();
	CTurnMgr::Get_Instance()->Initialize();
	CInven::Get_Instance()->Initialize();
	CObj* pPlayer = CAbFactory<CNewPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pPlayer);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pPlayer);
	/*CObj* pMonster_Melee = CAbFactory<CMonster_Melee>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Melee);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Melee);*/

	//CObj* pMonster_Range = CAbFactory<CMonster_Range>::Create();
	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Range);
	//CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Range);

	



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
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	CTileMgr::Get_Instance()->Render(_DC);
	CSight::Get_Instance()->Render(_DC);
	
	CObjMgr::Get_Instance()->Render(_DC);
	CInven::Get_Instance()->Render(_DC);  //CObjMgr보다 먼저 호출하면 플레이어가 인벤토리 위에 그려져서 부자연스러워진다

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
		TCHAR szStat[64];
		wsprintf(szStat, _T("ATK:%d DEF:%d"),
			pNewPlayer->Get_Atk(), pNewPlayer->Get_Def());
		SetBkMode(_DC, TRANSPARENT);
		SetTextColor(_DC, RGB(255, 255, 0));
		TextOut(_DC, 10, 30, szStat, lstrlen(szStat));
	}

}
void CStage::Release()
{

}