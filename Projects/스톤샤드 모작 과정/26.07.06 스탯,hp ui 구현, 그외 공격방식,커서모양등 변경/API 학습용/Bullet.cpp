#include "framework.h"
#include "Bullet.h"

CBullet::CBullet()
	:m_eDir(BULLET::END)
{

}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tInfo.iCX = 10;
	m_tInfo.iCY = 10;

	m_fSpeed = 20.f;

}

int CBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += cosf(PI / 180.f * m_fAngle) * m_fSpeed;  
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Update_Rect();

	return OBJ_NOEVENT;
	
}
void CBullet::Late_Update()
{
	if (m_tRect.left <= 100 || m_tRect.right >= WINCX - 100
		|| m_tRect.top <= 100 || m_tRect.bottom >= WINCY - 100)      
		m_bDead = !m_bDead;
}

void CBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom); 
}

void CBullet::Release()
{
}
