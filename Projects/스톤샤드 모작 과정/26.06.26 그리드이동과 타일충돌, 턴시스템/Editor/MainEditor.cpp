#include "pch.h"
#include "MainEditor.h"
#include "LineMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CMainEditor::CMainEditor()
{

}

CMainEditor::~CMainEditor()
{
	Release();
}

void CMainEditor::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	
	CLineMgr::Get_Instance()->Initialize();

}

void CMainEditor::Update()
{
	if(CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		CLineMgr::Get_Instance()->Create_Line();
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
		CLineMgr::Get_Instance()->Save_Line();
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		CLineMgr::Get_Instance()->Load_Line();

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
}

void CMainEditor::Late_Update()
{

}
void CMainEditor::Render()
{
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);

	CLineMgr::Get_Instance()->Render(m_hDC);
}

void CMainEditor::Release()
{
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();

	CLineMgr::Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);

}