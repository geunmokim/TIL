#include "framework.h"
#include "Monster.h"
#include "ObjMgr.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.iCX = 100;   
	m_tInfo.iCY = 100;   

	m_fSpeed = 1.f;        

	m_pTarget = CObjMgr::Get_Instance()->Get_Player(); 

}

int CMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	
	float fX = m_pTarget->Get_Info().fX - m_tInfo.fX;  //플레이어와 몬스터의 x 거리
	float fY = m_pTarget->Get_Info().fY - m_tInfo.fY;  //플레이어와 몬스터의 y 거리
	float fDia = sqrtf(fX * fX + fY * fY);

	float fRad = acosf(fX / fDia); //acosf는 사실 0~180밖에 커버를 못하기 떄문에 위아래 구분도 스스로 못함 acosf 대신 atan2f를 사용하는데 이건 학습용이라 사용한것
	m_fAngle = fRad * 180.f / PI;

	if (m_tInfo.fY < m_pTarget->Get_Info().fY)
		m_fAngle *= -1.f;  //acosf가 0~180도 밖에 커버가 안되므로 -1.f를해서 커버 안되는 나머지 0~180를 커버함

	m_tInfo.fX += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
	m_tInfo.fY -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{

}

void CMonster::Render(HDC _DC)
{
	Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release()
{

}