#include "framework.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "TurnMgr.h"
#include "Sight.h"
#include "Inven.h"

int g_iClickRow = 0;
int g_iClickCol = 0;
bool g_bCursorOnMonster = false;

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
	
	//Back/BackBuffer는 게임 화면(메뉴, 스테이지 등)을 그릴 때 쓰는 도화지
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Back.bmp"), _T("Back"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/BackBuffer.bmp"), _T("BackBuffer")); 


	CSoundMgr::Get_Instance()->Initialize();

	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::LOGO);



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
	if (CSceneMgr::LOGO == CSceneMgr::Get_Instance()->Get_SceneID())
		return;

	HDC hBackDC = CBmpMgr::Get_Instance()->Find_Image(_T("Back"));
	HDC hBBackBufferDC = CBmpMgr::Get_Instance()->Find_Image(_T("BackBuffer"));

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBBackBufferDC, 0, 0, SRCCOPY);

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	//CObjMgr::Get_Instance()->Render(m_hDC);






}
void CMainGame::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CTurnMgr::Destroy_Instance();
	CSight::Destroy_Instance();
	CInven::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	
	CObjMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}