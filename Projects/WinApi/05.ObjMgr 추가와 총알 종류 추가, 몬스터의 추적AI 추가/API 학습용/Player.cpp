#include "framework.h"
#include "Player.h"
#include "Bullet.h"
#include "Shield.h"
#include "ObjMgr.h"
#include "ScrewBullet.h"
#include "GuideBullet.h"

CPlayer::CPlayer()
	:m_fPosinDis(0.f)
{
	ZeroMemory(&m_tPosin, sizeof(m_tPosin));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fX = 400.f;   
	m_tInfo.fY = 300.f;  

	m_tInfo.iCX = 100;  
	m_tInfo.iCY = 100; 

	
	m_fSpeed = 5.f;  

	m_fPosinDis = 100.f;
}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) 
	{
		m_fAngle += m_fSpeed;
	}
		

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
	
		m_fAngle -= m_fSpeed;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)  
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed * cosf(m_fAngle * (PI / 180)); 
		m_tInfo.fY = m_tInfo.fY - m_fSpeed * sinf(m_fAngle * (PI / 180));

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		
		m_tInfo.fX = m_tInfo.fX - m_fSpeed * cosf(m_fAngle * (PI / 180));
		m_tInfo.fY = m_tInfo.fY - m_fSpeed * -sinf(m_fAngle * (PI / 180));
		
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)         
		CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CBullet>());
	if (GetAsyncKeyState('A') & 0x8000)
		CObjMgr::Get_Instance()->Add_Object(OBJID::SHIELD, Create_Shield());
	if (GetAsyncKeyState('S') & 0x8000)
		CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CScrewBullet>());
	if (GetAsyncKeyState('D') & 0x8000)
		CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CGuideBullet>());

	Update_Rect();

	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{
	m_tPosin.x = (LONG)(m_tInfo.fX + cosf(m_fAngle * PI / 180) * m_fPosinDis); 
	m_tPosin.y = (LONG)(m_tInfo.fY - sinf(m_fAngle * PI / 180) * m_fPosinDis);
}
void CPlayer::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	MoveToEx(_DC,(int)m_tInfo.fX,(int)m_tInfo.fY, nullptr); 
	LineTo(_DC, m_tPosin.x, m_tPosin.y);           
}
void CPlayer::Release()
{

}


CObj* CPlayer::Create_Shield()
{
	return CAbFactory<CShield>::Create(this);
}