#include "framework.h"
#include "Player.h"
#include "Bullet.h"
#include "Shield.h"
#include "ObjMgr.h"
#include "ScrewBullet.h"
#include "GuideBullet.h"
#include "LineMgr.h"
CPlayer::CPlayer()
	:m_fPosinDis(0.f), m_fJumpPower(0.f), m_fJumpTime(0.f), m_bJump(false), m_fJumpY(0.f)
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
	
	m_fJumpPower = 50.f;
}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check();
	Jumping();

	
	/*if (GetAsyncKeyState(VK_LEFT) & 0x8000) 
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
		CObjMgr::Get_Instance()->Add_Object(OBJID::BULLET, Create_Bullet<CGuideBullet>());*/

	Update_Rect();

	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{
	/*m_tPosin.x = (LONG)(m_tInfo.fX + cosf(m_fAngle * PI / 180) * m_fPosinDis); 
	m_tPosin.y = (LONG)(m_tInfo.fY - sinf(m_fAngle * PI / 180) * m_fPosinDis);*/
}
void CPlayer::Render(HDC _DC)
{
	Update_Rect();
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	/*MoveToEx(_DC,(int)m_tInfo.fX,(int)m_tInfo.fY, nullptr); 
	LineTo(_DC, m_tPosin.x, m_tPosin.y);           */
}
void CPlayer::Release()
{

}


CObj* CPlayer::Create_Shield()
{
	return CAbFactory<CShield>::Create(this);
}

void CPlayer::Key_Check()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_tInfo.fX -= m_fSpeed;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_tInfo.fX += m_fSpeed;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_bJump)                //점프중이 아니라면
			m_fJumpY = m_tInfo.fY;   //m_fJumpY에 현재 y좌표를 넣는다

		m_bJump = true;
	}
}

void CPlayer::Jumping()
{
	float fY = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY); //&y인 이유는 collision_line에서 매개변수를 *로 받아 y의 값을 수정해서 넘겨줄거기 떄문

	if (m_bJump)
	{
		m_fJumpTime += 0.2f;
		m_tInfo.fY = m_fJumpY - (m_fJumpPower * m_fJumpTime - 0.5f * 9.8f * m_fJumpTime * m_fJumpTime);
		//m_fJumpY대신 m_tInfo.fY를 넣으면 안되는 이유는 m_tInfo.fY가 이 식을 통해 계속해서 값이 바뀌기 떄문
		//식대로 원하는 값을 얻기 위해서 초기 m_tInfo.fY의 값을 가지는 m_fJumpY를 사용한것
		//위의 식에서 사용된 9.8은 실제 중력가속도 (실제 물리 상수), 0.5는 등가속도 운동 공식의 계수라 하며
		//위 식의 형태가 포물선 점프 공식이라 한다
		if (bLineCol && m_tInfo.fY >= fY)  //충돌판정이 일어난 상태에서 플레이어의 y좌표가 충돌 좌표의 y좌표보다 크거나 같다는것이 true라면
		{
			m_bJump = false;
			m_tInfo.fY = fY;
			m_fJumpTime = 0.f;

		}
		
	}  

	else if (bLineCol)
		m_tInfo.fY = fY;

}