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

}
void CStage::Release()
{

}