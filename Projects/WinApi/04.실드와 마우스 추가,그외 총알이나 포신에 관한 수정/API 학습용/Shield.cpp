#include "framework.h"
#include "Shield.h"

CShield::CShield()
{

}

CShield::~CShield()
{
	Release();
}

void CShield::Initialize()
{
	m_tInfo.iCX = 30;
	m_tInfo.iCY = 30;

	m_fDis = 150;

	m_fSpeed = 10.f;

}

int CShield::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	// m_tInfo.fX = 플레이어x + cosf(각도) + m_fDis;
	m_tInfo.fX = m_pTarget->Get_Info().fX + cosf(m_fAngle * PI / 180.f) * m_fDis;
	m_tInfo.fY = m_pTarget->Get_Info().fY - sinf(m_fAngle * PI / 180.f) * m_fDis;

	m_fAngle += m_fSpeed;

	Update_Rect();
	return OBJ_NOEVENT;

}
void CShield::Late_Update()
{   //경계에 닿을 시
	if (m_tRect.left <= 100 || m_tRect.right >= WINCX - 100       //경계에 닿는걸 상정한 조건식
		|| m_tRect.top <= 100 || m_tRect.bottom >= WINCY - 100)
		m_bDead = !m_bDead;

}

void CShield::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

}

void CShield::Release()
{

}