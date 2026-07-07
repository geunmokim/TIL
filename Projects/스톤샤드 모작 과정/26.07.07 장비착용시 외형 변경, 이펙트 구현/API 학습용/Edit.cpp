#include "framework.h"
#include "Edit.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CMyEdit::CMyEdit()
	:m_iCurDrawID(0)
{

}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Tile.bmp"), _T("Tile"));
	CTileMgr::Get_Instance()->Initialize();

}

void CMyEdit::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down('0'))
		m_iCurDrawID = 0;   // 바닥
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
		m_iCurDrawID = 1;   // 바닥 변형
	if (CKeyMgr::Get_Instance()->Key_Down('2'))
		m_iCurDrawID = 2;   // 문 (정면, 세로 통로용)
	if (CKeyMgr::Get_Instance()->Key_Down('3'))
		m_iCurDrawID = 3;   // 문 (측면, 가로 통로용)
	if (CKeyMgr::Get_Instance()->Key_Down('4'))
		m_iCurDrawID = 9;   // 벽 (위)
	if (CKeyMgr::Get_Instance()->Key_Down('5'))
		m_iCurDrawID = 10;  // 벽 (오른쪽)
	if (CKeyMgr::Get_Instance()->Key_Down('6'))
		m_iCurDrawID = 11;  // 벽 (아래)
	if (CKeyMgr::Get_Instance()->Key_Down('7'))
		m_iCurDrawID = 12;  // 벽 (왼쪽)
	if (CKeyMgr::Get_Instance()->Key_Down('8'))
		m_iCurDrawID = 13;  // 벽돌
	if (CKeyMgr::Get_Instance()->Key_Down('9'))
		m_iCurDrawID = 14;  // 벽돌


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(m_iCurDrawID);

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
		CTileMgr::Get_Instance()->Save_Tile();
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		CTileMgr::Get_Instance()->Load_Tile();
}

void CMyEdit::Late_Update()
{
}

void CMyEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
}

void CMyEdit::Release()
{
}
