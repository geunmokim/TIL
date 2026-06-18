#include "framework.h"
#include "Monster.h"
//현재 몬스터의 기능은 좌우로 움직이며 벽에 부딪힐 시 반대 방향으로 움직이게끔 설계
//intersectRect(#1, #2, #3): 충돌 체크 함수
//두 Rect가 충돌일 경우 true 반환, 충돌이 아닐 경우 false 반환
//#1: 충돌 범위의 Rect 좌표를 저장할 Rect 주소
//->RECT rc = {};
//#2: 충돌 검사할 Rect의 주소
//#3: 충돌 검사할 Rect의 주소

CMonster::CMonster()
{

}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	//m_tInfo.fX = 400.f;    //몬스터의 시작 위치 x좌표
	//m_tInfo.fY = 170.f;    //몬스터의 시작 위치 y좌표
	m_tInfo.iCX = 100;    //몬스터의 x축 크기
	m_tInfo.iCY = 100;    //몬스터의 y축 크기

	m_fSpeed = 5.f;        //몬스터의 이동 속도
	//m_eDir = MONSTER::RIGHT;// 처음 시작할때 어느방향으로 움직일지

}

int CMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.fX += m_fSpeed;

	//switch (m_eDir)
	//{
	//case MONSTER::LEFT:
	//	m_tInfo.fX -= m_fSpeed;
	//	break;
	//case MONSTER::RIGHT:
	//	m_tInfo.fX += m_fSpeed;
	//	break;
	//default:
	//	break;
	//}
	Update_Rect();
	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
	//if (m_eDir == MONSTER::LEFT && m_tRect.left <= 100)  //왼쪽으로 움직이던중 벽에 닿았을 경우
	//{
	//	m_eDir == MONSTER::RIGHT; //오른쪽으로 방향을 바꿔서 움직여라
	//}
	//if (m_eDir == MONSTER::RIGHT && m_tRect.right >= WINCX - 100)  //오른쪽으로 움직이던중 오른쪽 벽에 닿았을 경우
	//{
	//	m_eDir = MONSTER::LEFT;  //왼쪽방향으로 바꿔서 움직여라
	//}
	if (m_tRect.right >= (WINCX - 100)
		|| m_tRect.left <= 100)
		m_fSpeed *= -1.f;  //몬스터가 기존 5.f의 속도로 움직이고 있었으므로 -1.f를 곱하여 반대 방향으로 움직이게 함
}

void CMonster::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release()
{

}