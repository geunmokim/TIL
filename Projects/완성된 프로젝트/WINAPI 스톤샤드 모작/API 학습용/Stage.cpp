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
#include "Bandit_Cleaver.h"
#include "Ghoul.h"
#include "AncientTroll.h"
#include "Prop.h"
#include "SoundMgr.h"
CStage::CStage()
{

}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{   
	//½ºÅ×ÀÌÁö Æò»ó½Ã BGM Àç»ý //ÀÌÀü BGMÀº ²ô°í ½ÃÀÛ
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(_T("snd_battle_theme.ogg"));

	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Tile.bmp"), _T("Tile"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/UI/hp_bg_red.bmp"), _T("HP_BG"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/UI/hp_fill_red.bmp"), _T("HP_FILL"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/cleaver_1.bmp"), _T("Cleaver_Cursor"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/cleaver_0.bmp"), _T("Char_Cleaver"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/gambeson_1.bmp"), _T("Char_Gambeson"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/CharEquip/hat_1.bmp"), _T("Char_Hat"));

	CTileMgr::Get_Instance()->Load_Tile();
	CSight::Get_Instance()->Initialize();
	CTurnMgr::Get_Instance()->Initialize();
	CInven::Get_Instance()->Initialize();
	CObj* pPlayer = CAbFactory<CNewPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, pPlayer);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pPlayer);

	//stage Àç½ÃÀÛ½Ã ¯—¸Þ¶ó°¡ Á×¾úÀ»¶§ À§Ä¡¿¡ ¸ØÃçÀÖ´ø ¹®Á¦ -> ÇÃ·¹ÀÌ¾î ½ºÆù À§Ä¡·Î Ä«¸Þ¶ó ÀÌµ¿
	float fTargetScrollX = -(pPlayer->Get_Info().fX - WINCX / 2);
	float fTargetScrollY = -(pPlayer->Get_Info().fY - WINCY / 2);
	CScrollMgr::Get_Instance()->Set_ScrollX_To(fTargetScrollX);
	CScrollMgr::Get_Instance()->Set_ScrollY_To(fTargetScrollY);


	CObj* pMonster_cleaver = CAbFactory<CBandit_Cleaver>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_cleaver);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_cleaver);

	CObj* pMonster_Range = CAbFactory<CMonster_Range>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Range);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Range);

	CGhoul* pGhoul_Small = new CGhoul(GHOUL_SMALL);
	pGhoul_Small->Initialize();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pGhoul_Small);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pGhoul_Small);

	CGhoul* pGhoul_Large = new CGhoul(GHOUL_LARGE);
	pGhoul_Large->Initialize();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pGhoul_Large);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pGhoul_Large);

	CObj* pMonster_Ancienttroll = CAbFactory<CAncientTroll>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, pMonster_Ancienttroll);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pMonster_Ancienttroll);


	CProp* pProp1 = new CProp(PROP_BASTIONTABLE03);
	pProp1->Initialize();
	pProp1->Set_GridPos(7, 37);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp1);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp1);

	CProp* pProp2 = new CProp(PROP_BASTIONTABLE04);
	pProp2->Initialize();
	pProp2->Set_GridPos(4, 44);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp2);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp2);

	CProp* pProp3 = new CProp(PROP_PAWNSHOP_4);
	pProp3->Initialize();
	pProp3->Set_GridPos(10, 42);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp3);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp3);

	CProp* pProp4 = new CProp(PROP_PAWNSHOP_5);
	pProp4->Initialize();
	pProp4->Set_GridPos(5, 32);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp4);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp4);

	CProp* pProp5 = new CProp(PROP_PAWNSHOP_0);
	pProp5->Initialize();
	pProp5->Set_GridPos(3, 15);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp5);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp5);

	CProp* pProp6 = new CProp(PROP_PAWNSHOP_1);
	pProp6->Initialize();
	pProp6->Set_GridPos(10, 15);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp6);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp6);

	CProp* pProp7 = new CProp(PROP_PAWNSHOP_2);
	pProp7->Initialize();
	pProp7->Set_GridPos(22, 15);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp7);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp7);

	CProp* pProp8 = new CProp(PROP_PAWNSHOP_3);
	pProp8->Initialize();
	pProp8->Set_GridPos(21, 3);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PROP, pProp8);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::OBJECT, pProp8);

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
	//¸¶¿ì½º Ä¿¼­ ¼û±â±â¿ë
	static bool bPrevCursor = true;

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	CTileMgr::Get_Instance()->Render(_DC);
	CSight::Get_Instance()->Render(_DC);
	
	CObjMgr::Get_Instance()->Render(_DC);
	CInven::Get_Instance()->Render(_DC);  //CObjMgrº¸´Ù ¸ÕÀú È£ÃâÇÏ¸é ÇÃ·¹ÀÌ¾î°¡ ÀÎº¥Åä¸® À§¿¡ ±×·ÁÁ®¼­ ºÎÀÚ¿¬½º·¯¿öÁø´Ù


	if (g_bCursorOnMonster)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		
		// Ä¿¼­ ¼û±â±â (»óÅÂ ¹Ù²ð¶§¸¸)
		if (bPrevCursor)
		{
			ShowCursor(FALSE);
			bPrevCursor = false;
		}

		HDC hCursorDC = CBmpMgr::Get_Instance()->Find_Image(_T("Cleaver_Cursor"));
		GdiTransparentBlt(_DC, pt.x - 24, pt.y - 20, 48, 40,
			hCursorDC, 0, 0, 48, 40, RGB(255, 255, 255));


	}
	else if (!g_bCursorOnMonster && !bPrevCursor)
	{
		ShowCursor(TRUE);
		bPrevCursor = true;
	}




	//hp¹Ù 
	CObj* pPlayerHp = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayerHp)
	{


		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayerHp;

		HDC hBgDC = CBmpMgr::Get_Instance()->Find_Image(L"HP_BG");
		HDC hFillDC = CBmpMgr::Get_Instance()->Find_Image(L"HP_FILL");

		int iX = 10;
		int iY = 30;

		// ¹è°æ ÀüÃ¼
		BitBlt(_DC, iX, iY, 167, 8, hBgDC, 0, 0, SRCCOPY);

		// HP ºñÀ²¸¸Å­ Ã¤¿ò
		int iHpWidth = (int)(167.f * ((float)pNewPlayer->Get_Hp() / pNewPlayer->Get_MaxHp()));
		if (iHpWidth > 0)
			BitBlt(_DC, iX, iY, iHpWidth, 8, hFillDC, 0, 0, SRCCOPY);
	}


	

	//ÇÃ·¹ÀÌ¾î ½ºÅÈÀ» ÅØ½ºÆ®·Î Ç¥Çö
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
		TCHAR sz[64];
		wsprintf(sz, _T("HP:%d ATK:%d DEF:%d EXP:%d LV:%d"),
			pNewPlayer->Get_Hp(),
			pNewPlayer->Get_Atk(),
			pNewPlayer->Get_Def(),
			pNewPlayer->Get_Exp(),
			pNewPlayer->Get_Level());
		SetBkMode(_DC, TRANSPARENT);
		SetTextColor(_DC, RGB(255, 255, 0));
		TextOut(_DC, 10, 10, sz, lstrlen(sz));
	}

}
void CStage::Release()
{
	//½ºÅ×ÀÌÁö¿¡¼­ ¸¸µç ¿ÀºêÁ§Æ®µéÀ» ¾À ÀüÈ¯ ½Ã Á¤¸®
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::EFFECT);
	CObjMgr::Get_Instance()->Delete_ID(OBJID::ARROW);
	
	CObjMgr::Get_Instance()->Delete_ID(OBJID::PROP);

}