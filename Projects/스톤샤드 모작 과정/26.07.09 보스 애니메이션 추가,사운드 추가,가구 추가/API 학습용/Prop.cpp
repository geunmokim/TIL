#include "framework.h"
#include "Prop.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Sight.h"

CProp::CProp()
	:m_iSrcCX(0), m_iSrcCY(0), m_iDestCX(0), m_iDestCY(0)
{

}
CProp::CProp(PROP_TYPE _eType)
	:m_eType(_eType), m_iSrcCX(0),m_iSrcCY(0), m_iDestCX(0), m_iDestCY(0)
{

}
CProp::~CProp()
{

}

void CProp::Initialize()
{
	
	Initialize(m_eType);
}

void CProp::Initialize(PROP_TYPE _eType)
{
	m_eType = _eType;

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	if (m_eType == PROP_BASTIONTABLE03)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/bastiontable03.bmp"), _T("Prop_BastionTable03"));
		m_iSrcCX = 133;
		m_iSrcCY = 91;
		m_iDestCX = 160;
		m_iDestCY = 109;
		m_pImageKey = _T("Prop_BastionTable03");
	}
	else if (m_eType == PROP_BASTIONTABLE04)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/bastiontable04.bmp"), _T("Prop_BastionTable04"));
		m_iSrcCX = 87;
		m_iSrcCY = 170;
		m_iDestCX = 104;
		m_iDestCY = 204;
		m_pImageKey = _T("Prop_BastionTable04");
	}
	else if (m_eType == PROP_PAWNSHOP_0)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_0.bmp"), _T("Prop_Pawnshop_0"));
		m_iSrcCX = 61;
		m_iSrcCY = 49;
		m_iDestCX = 73;
		m_iDestCY = 59;
		m_pImageKey = _T("Prop_Pawnshop_0");
	}
	else if (m_eType == PROP_PAWNSHOP_1)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_1.bmp"), _T("Prop_Pawnshop_1"));
		m_iSrcCX = 58;
		m_iSrcCY = 49;
		m_iDestCX = 70;
		m_iDestCY = 59;
		m_pImageKey = _T("Prop_Pawnshop_1");
	}
	else if (m_eType == PROP_PAWNSHOP_2)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_2.bmp"), _T("Prop_Pawnshop_2"));
		m_iSrcCX = 56;
		m_iSrcCY = 33;
		m_iDestCX = 67;
		m_iDestCY = 40;
		m_pImageKey = _T("Prop_Pawnshop_2");
	}
	else if (m_eType == PROP_PAWNSHOP_3)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_3.bmp"), _T("Prop_Pawnshop_3"));
		m_iSrcCX = 56;
		m_iSrcCY = 36;
		m_iDestCX = 67;
		m_iDestCY = 43;
		m_pImageKey = _T("Prop_Pawnshop_3");
	}
	else if (m_eType == PROP_PAWNSHOP_4)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_4.bmp"), _T("Prop_Pawnshop_4"));
		m_iSrcCX = 54;
		m_iSrcCY = 40;
		m_iDestCX = 65;
		m_iDestCY = 48;
		m_pImageKey = _T("Prop_Pawnshop_4");
	}
	else //PROP_PAWNSHOP_5
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Object/pawnshop_5.bmp"), _T("Prop_Pawnshop_5"));
		m_iSrcCX = 54;
		m_iSrcCY = 40;
		m_iDestCX = 65;
		m_iDestCY = 48;
		m_pImageKey = _T("Prop_Pawnshop_5");
	}

	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
}

int CProp::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CProp::Late_Update()
{

}

void CProp::Render(HDC _DC)
{
	if (CSight::Get_Instance()->Get_Visibility(m_tGridPos.row, m_tGridPos.col) != VISIBLE)
		return;

	HDC hPropDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollY();

	int iDestX = m_tRect.left + iScrollX;
	int iDestY = m_tRect.top + iScrollY;

	GdiTransparentBlt(_DC,
		iDestX, iDestY, m_iDestCX, m_iDestCY,
		hPropDC, 0, 0, m_iSrcCX, m_iSrcCY,
		RGB(255, 255, 255));
}

void CProp::Release()
{

}