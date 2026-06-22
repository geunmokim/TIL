#include "framework.h"
#include "Player.h"
#include "Bullet.h"
#include "Shield.h"
#include "ObjMgr.h"
#include "ScrewBullet.h"
#include "GuideBullet.h"
#include "LineMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"

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
{//플레이어같이 하나만 사용되는 것은 이니셜라이즈에서 이미지를 불러와도 되지만
  //몬스터 같이 여러개쓰이는건 이니셜라이즈에서 불러오면 안된다
	//화면이 깜빡이는 이유는 계속 업데이트 되기 떄문
	//-> 최소화를 통해 줄이자
	//유한 상태 기계 --- 형광등으로 생각하면 이해하기 쉽다
	//m_ePreScene !- m_eCurScene  ---- 기존의 씬과 현재 불러올 씬이 다르다면 --- 로고에서 메뉴로 바꿀때
	/*기존 정보와 불러올 정보가 다르다는 조건을 만족한다면 */

	//버튼이랑 키 충돌 함수 대신 사용하는거 ---PtInRect() //
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
	OffSet();
	

	Update_Rect();

	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{
	Key_Check();
	Jumping();
	OffSet();
	Update();
}
void CPlayer::Render(HDC _DC)
{
	Update_Rect();
	HDC hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(_T("Player"));
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top, m_tRect.right + iScrollX, m_tRect.bottom);


	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hPlayerDC
		, 0, 0
		, 60, 60
		, RGB(255, 255, 255));




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
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) )
	{
		m_tInfo.fX -= m_fSpeed;
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
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

void CPlayer::OffSet()
{
	int iOffsetX = WINCX >> 1;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffsetX > (int)m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
	if (iOffsetX < (int)m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + iScrollX));
}