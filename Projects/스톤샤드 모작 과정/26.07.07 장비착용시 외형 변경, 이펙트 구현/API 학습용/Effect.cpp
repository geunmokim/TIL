#include "framework.h"
#include "Effect.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CEffect::CEffect()
    :m_eEffectType(EFFECT_END), m_iFrameW(0), m_iFrameH(0)
{

}

CEffect::CEffect(EFFECT_TYPE _eType)
    :m_eEffectType{_eType}, m_iFrameW(0), m_iFrameH(0)
{

}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{

    if (m_eEffectType == EFFECT_ATTACK)
    {
        CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Effect/attack_effect.bmp"), _T("AttackEffect"));

        m_tInfo.iCX = TILECX;
        m_tInfo.iCY = TILECY;
        m_iFrameW = 77;
        m_iFrameH = 68;
        m_pImageKey = _T("AttackEffect");
        m_eRenderID = RENDERID::EFFECT;
        m_bDead = false;
        // 프레임 설정
        m_tFrame.iStartX = 0;    // 시작 프레임
        m_tFrame.iEndX = 7;    // 총 프레임 수
        m_tFrame.iStateY = 0;    // 행 (단일 행이라 0)
        m_tFrame.dwDelayTime = 40;  // 프레임 간격 (ms)
        m_tFrame.dwTime = GetTickCount();
    }
    else //EFFECT_HIT
    {
        CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Effect/hit_effect.bmp"), _T("HitEffect"));
        m_tInfo.iCX = TILECX;
        m_tInfo.iCY = TILECY;
        m_iFrameW = 37;
        m_iFrameH = 37;
        m_pImageKey = _T("HitEffect");
        m_eRenderID = RENDERID::EFFECT;
        m_bDead = false;
        m_tFrame.iStartX = 0;
        m_tFrame.iEndX = 7;
        m_tFrame.iStateY = 0;
        m_tFrame.dwDelayTime = 40;
        m_tFrame.dwTime = GetTickCount();
    }
}

int CEffect::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    Update_Rect();
    if (m_tFrame.dwTime + m_tFrame.dwDelayTime < GetTickCount())
    {
        ++m_tFrame.iStartX;
        m_tFrame.dwTime = GetTickCount();
    }

    if (m_tFrame.iStartX >= m_tFrame.iEndX)
    {
        m_bDead = true;
        return OBJ_DEAD;
    }
    return OBJ_NOEVENT;


}

void CEffect::Late_Update()
{

}

void CEffect::Render(HDC _DC)
{
  
    int iSrcX = m_tFrame.iStartX * 77;
    HDC hEffectDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    int iDestX = m_tRect.left + iScrollX;
    int iDestY = m_tRect.top + iScrollY;

    GdiTransparentBlt(_DC,
        iDestX, iDestY, m_iFrameW, m_iFrameH,
        hEffectDC,
        iSrcX, 0, m_iFrameW, m_iFrameH,
        RGB(255, 255, 255));
}

void CEffect::Release()
{

}