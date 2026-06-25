#include "framework.h"
#include "Edit.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CMyEdit::CMyEdit()
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
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(1);
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		CTileMgr::Get_Instance()->Picking_Tile(0);

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
