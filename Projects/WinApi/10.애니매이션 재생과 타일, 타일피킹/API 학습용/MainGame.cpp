#include "framework.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"
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

	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Back.bmp"), _T("Back"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/BackBuffer.bmp"), _T("BackBuffer"));

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::LOGO);


	//CLineMgr::Get_Instance()->Initialize();

	//CObj* pObj = CAbFactory<CPlayer>::Create();
	//CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pObj);

	

}

void CMainGame::Update()
{
	CSceneMgr::Get_Instance()->Update();
	//CObjMgr::Get_Instance()->Update();
}
void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
	//CObjMgr::Get_Instance()->Late_Update();

}

void CMainGame::Render()
{
	//Rectangle(m_hDC, 0, 0, WINCX, WINCY);                    

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(_T("Back"));
	HDC hBBackBufferDC = CBmpMgr::Get_Instance()->Find_Image(_T("BackBuffer"));

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBBackBufferDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	//CObjMgr::Get_Instance()->Render(m_hDC);
	//CLineMgr::Get_Instance()->Render(m_hDC);

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
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();


	CObjMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}