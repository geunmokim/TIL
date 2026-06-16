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

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

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
		m_pBullet->emplace_back(Create_Bullet(BULLET::UP));
	}
	if (GetAsyncKeyState('A') & 0x8000)           //A눌렀을떄 키 감지
	{
		m_pBullet->emplace_back(Create_Bullet(BULLET::LEFT));   
	}
	if (GetAsyncKeyState('S') & 0x8000)          //S 눌렀을 때 아래로 총알 발사
	{
		m_pBullet->emplace_back(Create_Bullet(BULLET::DOWN));
	}
	if (GetAsyncKeyState('D') & 0x8000)          //D 눌렀을 때  오른쪽으로 총알 발사
	{
		
		m_pBullet->emplace_back(Create_Bullet(BULLET::RIGHT));
	}

	Update_Rect();

	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{

}
void CPlayer::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}
void CPlayer::Release()
{

}


CObj* CPlayer::Create_Bullet(BULLET::DIR _eDir)   //다형성 덕분에 같은 부모CObj의 다른 자식 Bullet의 함수 사용 가능
{
	//아래 내용은 abstract factory에서 정의했으므로 주석처리
	//CObj* pObj = new CBullet;
	//pObj->Initialize();
	//pObj->Set_Pos(m_tInfo.fX, m_tInfo.fY);                 //플레이어의 위치에서 생성 -> 이거 때문에 Create_Bullet()을 Player()에서 생성함

	/*
      만약 CBullet에서 만들면:
      CBullet이 플레이어 위치를 알려면
      Player를 참조해야 함
      #include "Player.h"  // Player가 Bullet 포함하고
					       // Bullet이 Player 포함하면
					       // 순환 참조 발생!
*/
	CObj* pObj = CAbFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY);
	static_cast<CBullet*>(pObj)->Set_Dir(_eDir);

	return pObj;

}