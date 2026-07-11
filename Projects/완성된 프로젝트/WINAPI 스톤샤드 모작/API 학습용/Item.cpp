#include "framework.h"
#include "Item.h"
#include "BmpMgr.h"



CItem::CItem()
	:m_iWidth(0),m_iHeight(0),m_iGridX(0),m_iGridY(0)
{

}
CItem::CItem(const ITEM_INFO& _tInfo)
    : m_iWidth(_tInfo.iWidth)
    , m_iHeight(_tInfo.iHeight)
    , m_iSrcWidth(_tInfo.iSrcWidth)
    , m_iSrcHeight(_tInfo.iSrcHeight)
    , m_eType(_tInfo.eType)
    , m_iHp(_tInfo.iHp)
    , m_iDef(_tInfo.iDef)
    , m_iAtk(_tInfo.iAtk)
    , m_pPath(_tInfo.pPath)
    , m_pImageKey(_tInfo.pImageKey)
    , m_iGridX(0)
    , m_iGridY(0)
{

}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(m_pPath, m_pImageKey);

	
}

void CItem::Render(HDC _DC, int _iPixelX, int _iPixelY)
{
    HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

    GdiTransparentBlt(_DC,
        _iPixelX, _iPixelY,
        m_iWidth * 27, m_iHeight * 27,   // 격자 칸 크기에 맞게 늘려서 그림
        hItemDC,
        0, 0,
        m_iSrcWidth, m_iSrcHeight,        // 원본 크기
        RGB(255, 255, 255));
}

void CItem::Render_Original(HDC _DC, int _iPixelX, int _iPixelY)
{
    HDC hItemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);
    GdiTransparentBlt(_DC,
        _iPixelX, _iPixelY,
        m_iSrcWidth, m_iSrcHeight,
        hItemDC,
        0, 0,
        m_iSrcWidth, m_iSrcHeight,
        RGB(255, 255, 255));
}
void CItem::Release()
{

}