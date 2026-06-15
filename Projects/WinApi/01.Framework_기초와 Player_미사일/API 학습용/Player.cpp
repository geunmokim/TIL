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

	m_fSpeed = 5.f;   //플레이어의 속도

}

void CPlayer::Update()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) //지정한 키를 눌렀을 때 움직임 && 키가 눌렸는지 확인
		m_tInfo.fX -= m_fSpeed;   //총알과 다르게 플레이어는 내가 눌렀을때만 움직여야하므로 GetAsynKeyState로 움직임 설정

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_tInfo.fX += m_fSpeed;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_tInfo.fY -= m_fSpeed;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_tInfo.fY += m_fSpeed;

	if (GetAsyncKeyState('W') & 0x8000)            //w눌렀을떄 위로 총알 발사
	{
		My_AsyncKeyState('W');
		m_pBullet->emplace_back(Create_Bullet());
	}
	if (GetAsyncKeyState('A') & 0x8000)           //A눌렀을떄 키 감지
	{
		My_AsyncKeyState('A');
		m_pBullet->emplace_back(Create_Bullet());   
	}
	if (GetAsyncKeyState('S') & 0x8000)          //S 눌렀을 때 아래로 총알 발사
	{
		My_AsyncKeyState('S');
		m_pBullet->emplace_back(Create_Bullet());
	}
	if (GetAsyncKeyState('D') & 0x8000)          //D 눌렀을 때  오른쪽으로 총알 발사
	{
		My_AsyncKeyState('D');
		m_pBullet->emplace_back(Create_Bullet());
	}

	Update_Rect();

}
void CPlayer::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}
void CPlayer::Release()
{

}


CObj* CPlayer::Create_Bullet()   //다형성 덕분에 같은 부모CObj의 다른 자식 Bullet의 함수 사용 가능
{
	CObj* pObj = new CBullet;
	pObj->Initialize();
	pObj->Set_Pos(m_tInfo.fX, m_tInfo.fY);                 //플레이어의 위치에서 생성 -> 이거 때문에 Create_Bullet()을 Player()에서 생성함

	/*
      만약 CBullet에서 만들면:
      CBullet이 플레이어 위치를 알려면
      Player를 참조해야 함
      #include "Player.h"  // Player가 Bullet 포함하고
					       // Bullet이 Player 포함하면
					       // 순환 참조 발생!
*/

	static_cast<CBullet*>(pObj)->My_AsyncKeyState(m_Key);

	return pObj;

}