#include "framework.h"
#include "Stage.h"
#include "BmpMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "Monster.h"

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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Monster.bmp", L"Monster");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Reset.bmp", L"Reset");
	
	
	CTileMgr::Get_Instance()->Load_Tile();

	/*CObj* pObj = CAbFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);*/
	CObj* pObj = CAbFactory<CMonster>::Create(400.f, 200.f);
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

}

void CStage::Update()
{
	CObjMgr::Get_Instance()->Update();
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
	CObjMgr::Get_Instance()->Render(_DC);
}
void CStage::Release()
{

}