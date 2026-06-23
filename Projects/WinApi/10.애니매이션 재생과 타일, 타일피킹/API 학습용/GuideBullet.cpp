#include "framework.h"
#include "GuideBullet.h"
#include "ObjMgr.h"

CGuideBullet::CGuideBullet()
{

}

CGuideBullet::~CGuideBullet()
{
	Release();
}

void CGuideBullet::Initialize()
{
	m_tInfo.iCX = 10;
	m_tInfo.iCY = 10;

	m_fSpeed = 5.f;

}

int CGuideBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTarget = CObjMgr::Get_Instance()->Get_Target(OBJID::MONSTER, this);

	if (m_pTarget)
	{
		float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;
		float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;
		float fDia = sqrtf(fX * fX + fY * fY);

		float fRad = acosf(fX / fDia);
		m_fAngle = fRad * 180.f / PI;

		if (m_pTarget->Get_Info().fY > m_tInfo.fY)
			m_fAngle *= -1.f;
		

	}
	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;
	Update_Rect();
	return OBJ_NOEVENT;
}
void CGuideBullet::Late_Update()
{
	if (m_tRect.left <= 100 || m_tRect.right >= WINCX - 100
		|| m_tRect.top <= 100 || m_tRect.bottom >= WINCY - 100)
		m_bDead = !m_bDead;
}

void CGuideBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CGuideBullet::Release()
{

}