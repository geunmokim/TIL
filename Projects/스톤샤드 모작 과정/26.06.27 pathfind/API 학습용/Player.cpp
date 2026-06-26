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
	:m_fPosinDis(0.f), m_fJumpPower(0.f), m_fJumpTime(0.f), m_bJump(false), m_fJumpY(0.f),m_ePreState(END), m_eCurState(END)
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

	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Player/Player_DOWN.bmp"), _T("Player_Down"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Player/Player_UP.bmp"), _T("Player_Up"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Player/Player_LEFT.bmp"), _T("Player_Left"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Player/Player_RIGHT.bmp"), _T("Player_Right"));

	m_tInfo.fX = 400.f;   
	m_tInfo.fY = 300.f;  

	m_tInfo.iCX = 200;  
	m_tInfo.iCY = 200; 

	
	m_fSpeed = 5.f;  

	m_fPosinDis = 100.f;
	
	m_fJumpPower = 50.f;

	m_pImageKey = _T("Player_Down");
	m_eCurState = IDLE;
}

int CPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check();
	Jumping();
	OffSet();
	Frame_Check();

	Update_Frame();
	Update_Rect();

	return OBJ_NOEVENT;

}
void CPlayer::Late_Update()
{
	/*Key_Check();
	Jumping();
	OffSet();
	Update();*/
}
void CPlayer::Render(HDC _DC)
{
	Update_Rect();
	//HDC hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(_T("Player_Down"));
	HDC		hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top, m_tRect.right + iScrollX, m_tRect.bottom);


	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hPlayerDC
		, m_tFrame.iStartX * 200, m_tFrame.iStateY * 200 //사진크기 1200 x 1000
		, 200, 200
		, RGB(0, 0, 0));




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
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		m_eCurState = WALK;
		m_pImageKey = _T("Player_Left");
		//CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);   // 왼쪽 이동 → ScrollX는 + -> 화면좌표 = 월드좌표 + ScrollX
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		m_eCurState = WALK;
		m_pImageKey = _T("Player_Right");
		//CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed); //오른쪽 이동 -> ScrollX는 - -> 월드좌표 = 화면좌표 - ScrollX
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_tInfo.fY -= m_fSpeed;
		m_eCurState = WALK;
		m_pImageKey = _T("Player_Up");

	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_tInfo.fY += m_fSpeed;
		m_eCurState = WALK;
		m_pImageKey = _T("Player_Down");

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		m_eCurState = ATTACK;
		m_pImageKey = _T("Player_Attack");

	}
	else
		m_eCurState = IDLE;


	//if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	//{
	//	if (!m_bJump)                //점프중이 아니라면
	//		m_fJumpY = m_tInfo.fY;   //m_fJumpY에 현재 y좌표를 넣는다

	//	m_bJump = true;
	//}
}

void CPlayer::Jumping()
{
	float fY = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY); //&y인 이유는 collision_line에서 매개변수를 *로 받아 y의 값을 수정해서 넘겨줄거기 떄문

	if (m_bJump)
	{
		m_fJumpTime += 0.2f;
		m_tInfo.fY = m_fJumpY - (m_fJumpPower * m_fJumpTime - 0.5f * 9.8f * m_fJumpTime * m_fJumpTime);
		
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

void CPlayer::Frame_Check()
{
	if (m_ePreState != m_eCurState) //전모션과 현모션이 다르다면
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iStartX = 0;
			m_tFrame.iEndX = 4;
			m_tFrame.iStateY = IDLE;
			m_tFrame.dwDelayTime = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		case CPlayer::WALK:
			m_tFrame.iStartX = 0;               //스프라이트 시트에서 잘라올 칸의 번호  //지금 현재 보여주고 있는 프레임의 번호인데 사실상 0부터 시작함
			m_tFrame.iEndX = 6;                //이 애니메이션이 총 몇 개의 프레임으로 구성되어 있는지(개수) //bmp나 png로 해당 모션이 몇개로 나눠져있는지를 보고 정하는값
			m_tFrame.iStateY = WALK;
			m_tFrame.dwDelayTime = 50;         //워크 상태면 50ms마다 갱신
			m_tFrame.dwTime = GetTickCount();  //워크 발동 시의 시각을 저장
			break;
		case CPlayer::ATTACK:
		/*	m_tFrame.iStartX = 0;           
			m_tFrame.iEndX = 6;             
			m_tFrame.iStateY = ATTACK;
			m_tFrame.dwDelayTime = 200;          
			m_tFrame.dwTime = GetTickCount(); */
			break;
		case CPlayer::HIT:
				break;
			case CPlayer::DEAD:
				break;
/*
void CObj::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwDelayTime < GetTickCount())
	{
		++m_tFrame.iStartX;

		if (m_tFrame.iStartX >= m_tFrame.iEndX)
			m_tFrame.iStartX = 0;

		m_tFrame.dwTime = GetTickCount();

	}
}

*/

		}
		m_ePreState = m_eCurState;
	}
}