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

	m_tInfo.fX += cosf(PI / 180.f * m_fAngle) * m_fSpeed;  //포신 끝에서 나가는 것처럼 보이게 하기 위해 포신과 방향을 맞췄음
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Update_Rect();

	return OBJ_NOEVENT;
	
}
void CBullet::Late_Update()
{
	if (m_tRect.left <= 100 || m_tRect.right >= WINCX - 100
		|| m_tRect.top <= 100 || m_tRect.bottom >= WINCY - 100)      //총알이 해당 조건들을 벗어나게 된다면 소멸
		m_bDead = !m_bDead;
}

void CBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left/*(LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1)) */, m_tRect.top, m_tRect.right, m_tRect.bottom); //총알의 모양
}

void CBullet::Release()
{
}
