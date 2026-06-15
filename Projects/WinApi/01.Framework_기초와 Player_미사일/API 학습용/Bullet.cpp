#include "framework.h"
#include "Bullet.h"

CBullet::CBullet()
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

void CBullet::Update()
{
	//if (GetAsyncKeyState(0x57) & 0x8000) //W일때 위로 발사  -> 내가 누를때만 스피드만큼 이동하는 것이기 때문에 내가 버튼에서 손을 떼면 멈춰버림 그래서 잔상이 남는것 처럼 보인다
//	m_tInfo.fY -= m_fSpeed;
//if (GetAsyncKeyState(0x41) & 0x8000) //A일때 왼쪽으로 발사
//	m_tInfo.fX -= m_fSpeed;          
//if (GetAsyncKeyState(0x53) & 0x8000) 
//	m_tInfo.fY += m_fSpeed;          
//if (GetAsyncKeyState(0x44) & 0x8000)
//	m_tInfo.fX += m_fSpeed;                
// ------------ 해본 결과 총알이 내가 누를때만 나가기 때문에  눌렀다 떼면 잔상이 남음 --> 스피드 만큼 이동하기 때문에 잔상이 남는거 같다 + 총알 나가는거 동시에 누르면 대각선으로 나감 


	if (m_Key == 'W')            //if(m_Key == 0x57)
		m_tInfo.fY -= m_fSpeed;
	if (m_Key == 'A')            //if(m_Key == 0x41)
		m_tInfo.fX -= m_fSpeed;
	if (m_Key == 'S')            //if(m_Key == 0x53)
		m_tInfo.fY += m_fSpeed;
	if (m_Key == 'D')            //if(m_Key == 0x44)와 같다
		m_tInfo.fX += m_fSpeed;


	Update_Rect();
	
}


void CBullet::Render(HDC _DC)
{
	Ellipse(_DC, m_tRect.left/*(LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1)) */, m_tRect.top, m_tRect.right, m_tRect.bottom); //총알의 모양
}

void CBullet::Release()
{
}
