#include "framework.h"
#include "Menu.h"
#include "BmpMgr.h"
#include "Button.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
//실제로 button을 생성, 관리 하는 곳
CMyMenu::CMyMenu()
{

}

CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize()
{
	// 메뉴 BGM 재생 //이전에 재생 중이던 BGM은 끄고 시작
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(_T("snd_mus_dungeon_03_short_05.ogg"));

	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Menu/Menu.bmp"), _T("Menu"));  //_T(" ")라서 ()까지 붙여줘야 L" "과 같은 기능을 한다
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Button/Start.bmp"), _T("Start"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Button/Edit.bmp"), _T("Edit"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Button/Exit.bmp"), _T("Exit"));

	CObj* pObj = CAbFactory<CMyButton>::Create(200.f, 400.f);
	pObj->Set_ImageKey(_T("Start"));
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbFactory<CMyButton>::Create(400.f, 400.f);
	pObj->Set_ImageKey(_T("Edit"));
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

	pObj = CAbFactory<CMyButton>::Create(600.f, 400.f);
	pObj->Set_ImageKey(L"Exit");
	CObjMgr::Get_Instance()->Add_Object(OBJID::UI, pObj);

}

void CMyMenu::Update()
{
	CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();

}

void CMyMenu::Render(HDC _DC)
{
	HDC hMenuDC = CBmpMgr::Get_Instance()->Find_Image(_T("Menu"));
	StretchBlt(_DC, 0, 0, WINCX, WINCY,
		hMenuDC, 0, 0, 1148, 326, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC);
}

void CMyMenu::Release()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJID::UI);
}