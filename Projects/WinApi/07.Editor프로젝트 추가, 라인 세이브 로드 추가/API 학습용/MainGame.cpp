#include "framework.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
CMainGame::CMainGame()
	: m_pPlayer(nullptr),m_iFPS(0), m_dwTime(GetTickCount())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CLineMgr::Get_Instance()->Initialize();

	CObj* pObj = CAbFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	//pObj = CAbFactory<CMonster>::Create(200.f, 200.f);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

	//pObj = CAbFactory<CMonster>::Create(600.f, 200.f);
	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);

 //   pObj = CAbFactory<CMonster>::Create(200.f, 200.f);  
	//CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pObj);
	////m_listObj[OBJID::MONSTER].emplace_back(pObj);

}

void CMainGame::Update()
{
	CObjMgr::Get_Instance()->Update();
}
void CMainGame::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

}

void CMainGame::Render()
{
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);                    
	//Rectangle(m_hDC, 100, 100, WINCX - 100, WINCY - 100);

	CObjMgr::Get_Instance()->Render(m_hDC);
	CLineMgr::Get_Instance()->Render(m_hDC);

#ifdef _DEBUG
	TCHAR		szBuff[32] = L"";
	swprintf_s(szBuff, L"Bullet: %d", m_listObj[OBJID::BULLET].size());
	TextOut(m_hDC, 50, 50, szBuff, lstrlen(szBuff));

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		TCHAR	szFPS[16] = L"";
		swprintf_s(szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
	
#endif




}
void CMainGame::Release()
{
	CLineMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_hDC);
}