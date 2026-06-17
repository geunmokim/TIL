#include "framework.h"
#include "Player.h"
#include "Bullet.h"

CPlayer::CPlayer()
{
}
CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo.fX = 400.f;   //플레이어의 시작 x좌표
	m_tInfo.fY = 300.f;  //플레이어의 시작 y좌표

	m_tInfo.iCX = 100;  //플레이어의 x크기 ---400,300 좌표를 중심으로 (350,250), (350,350),(400,250),(400,350)의 네 꼭지점을 가지는 사각형을 그림
	m_tInfo.iCY = 100; //플레이어의 y크기

	m_iLengthY = m_tInfo.iCY + 10; // 포신의 길이 = 110;
	m_fSpeed = 5.f;   //플레이어의 속도

}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) //지정한 키를 눌렀을 때 움직임 && 키가 눌렸는지 확인
	{
		m_fAngle += m_fSpeed;
	}
		

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		//m_tInfo.fX += m_fSpeed;
		m_fAngle -= m_fSpeed;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)   //위의 키를 누를 시 포신의 방향으로 전진
	{
		m_tInfo.fX = m_tInfo.fX + m_fSpeed * -cosf(m_fAngle * (PI / 180)); //5의 속력으로 포신의 방향으로 움직임  화면은 Y축이 반전되어 있어서 cos에 음수를 붙여 방향을 맞춤
		m_tInfo.fY = m_tInfo.fY + m_fSpeed * sinf(m_fAngle * (PI / 180));

	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) //아래 키를 누를 시 포신의 끝의 반대 방향으로 전진(포신 방향으로 후진)
	{
		//cosf, sinf는 라디안을 받기 때문에 도(degree) 단위인 m_fAngle을 변환
		m_tInfo.fX = m_tInfo.fX - m_fSpeed * -cosf(m_fAngle * (PI / 180));
		m_tInfo.fY = m_tInfo.fY - m_fSpeed * sinf(m_fAngle * (PI / 180));
		
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)            //스페이스바를 누르면 총알이 생성, 발사
	{
		m_pBullet->emplace_back(Create_Bullet(BULLET::UP));
	}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	{
	//		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	//		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	//	}
	//	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	{
	//		m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	//		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	//	}
	//	else
	//		m_tInfo.fX -= m_fSpeed;
	//}
	//else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	{
	//		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	//		m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	//	}
	//	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	{
	//		m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	//		m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	//	}
	//	else
	//		m_tInfo.fX += m_fSpeed;
	//}

	Update_Rect();
	Barrel();
	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{

}
void CPlayer::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	MoveToEx(_DC, m_tInfo.fX, m_tInfo.fY, nullptr);
	LineTo(_DC, m_iBarrelX, m_iBarrelY);
}
void CPlayer::Release()
{

}


CObj* CPlayer::Create_Bullet(BULLET::DIR _eDir)   //다형성 덕분에 같은 부모CObj의 다른 자식 Bullet의 함수 사용 가능
{
	CObj* pObj = CAbFactory<CBullet>::Create(m_iBarrelX, m_iBarrelY);
	/*static_cast<CBullet*>(pObj)->Set_Dir(_eDir);*/ //방향 다 지워서 의미 없어짐
	static_cast<CBullet*>(pObj)->Set_MyAngle(m_fAngle);

	return pObj;

}

void CPlayer::Barrel()
{
	m_iBarrelX = m_tInfo.fX + -cosf(m_fAngle * (PI / 180)) * m_iLengthY;  //-cosf에 마이너스가 붙은 이유는 화면 좌표계 때문
	m_iBarrelY = m_tInfo.fY + sinf(m_fAngle * (PI / 180)) * m_iLengthY; //m_fAngle * (PI / 180) 은 각도값을 cosf, sinf가 이해할 수 있는 단위로 바꿔주는 것

}