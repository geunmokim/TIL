#include "framework.h"
#include "ScrewBullet.h"

CScrewBullet::CScrewBullet()
	:m_fCenterAngle(0.f), m_fCenterSpeed(0.f), m_bStart(false)
{
	ZeroMemory(&m_tCenterPos, sizeof(m_tCenterPos));

}

CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize()
{
	m_bStart = true;

	m_tInfo.iCX = 10; //iCX,iCY는 총알 크기 의미
	m_tInfo.iCY = 10;

	m_fSpeed = 30.f;
	m_fDis = 50.f;

	m_fCenterSpeed = 5.f;
}

int CScrewBullet::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	//총알의 생성 위치
	if (m_bStart)
	{
		m_fCenterAngle = m_fAngle;
		m_tCenterPos.x = (LONG)m_tInfo.fX; 
		m_tCenterPos.y = (LONG)m_tInfo.fY;
		/*
		m_tInfo.fX와 m_tInfo.fY는
		template <typename T>
	    CObj* Create_Bullet()
	    {
		return CAbFactory<T>::Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle);
	    }
		로 플레이어가 총알을 생성할때마다 m_tPosin.x,m_tPosin.y의 값을 받아와서 플레이어의 포신의끝x와 포신의 끝y에서 생성된다

		*/

		m_bStart = !m_bStart; //초기에 true이니 조건식 통과후 !로 false를 만들어 Update()를 실행해도 반복못하게함
	}


	//보이지 않는 중심점이 플레이어의 포신 방향으로 직진함
	m_tCenterPos.x += (LONG)(cosf(m_fCenterAngle * PI / 180.f) * m_fCenterSpeed);
	m_tCenterPos.y -= (LONG)(sinf(m_fCenterAngle * PI / 180.f) * m_fCenterSpeed);

	//중심점을 기준으로 총알이 회전함
	m_tInfo.fX = m_tCenterPos.x + cosf(m_fAngle * PI / 180.f) * m_fDis;
	m_tInfo.fY = m_tCenterPos.y - sinf(m_fAngle * PI / 180.f) * m_fDis;

	m_fAngle += m_fSpeed;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update()
{
	if(m_tRect.left <= 100 || m_tRect.right >= WINCX - 100
		|| m_tRect.top <= 100 || m_tRect.bottom >= WINCY - 100)
		m_bDead = !m_bDead;
}

void CScrewBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CScrewBullet::Release()
{
}
