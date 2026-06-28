#include "framework.h"
#include "Monster_Range.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "PathFind.h"
#include "Arrow.h"





CMonster_Range::CMonster_Range()
{

}
CMonster_Range::~CMonster_Range()
{

}

void CMonster_Range::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Range/s_bandit_outlaw01_0.bmp"), _T("Bandit_Outlaw"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	Set_GridPos(15, 15); // 시작 칸 위치 //일단 임의로 잡아둔 값

	m_pImageKey = _T("Bandit_Outlaw");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
	m_iAmmo = 999;
	m_iAttackRange = 3; //내가 정해준 현재 원거리 몬스터의 사거리
}


void CMonster_Range::On_Turn()
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player(); //플레이어의 정보를 가져와서 pPlayer에 저장


	GRIDPOS playerPos = pPlayer->Get_GridPos(); //player에 설정되있는 좌표를 가져와서 playerPos에 저장 -> find_nextStep에 사용하기 위해
	GRIDPOS monsterPos = Get_GridPos();

	int iDistance = monsterPos.Get_Distance(playerPos);

	//몬스터의 시야거리보다 거리가 멀때
	if (iDistance > MONSTER_SIGHT_RANGE)
		return;



	if (iDistance <= m_iAttackRange)  //플레이어와의 거리가 3이면 공격
	{
		Attack(pPlayer);
		m_iAmmo--;
	}
	else
	{
		GRIDPOS next = CPathFind::Find_NextStep(monsterPos, playerPos); //플레이어의 좌표를 받아와서 플레이어와의 경로 계산
		Chase(next);
	}




}

void CMonster_Range::Attack(CObj* _pTarget)
{
	GRIDPOS targetPos = _pTarget->Get_GridPos();

	CArrow* pArrow = (CArrow*)CAbFactory<CArrow>::Create();
	pArrow->Set_TargetPos(targetPos);
	CObjMgr::Get_Instance()->Add_Object(OBJID::ARROW, pArrow);
}

