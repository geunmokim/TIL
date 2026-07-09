#include "framework.h"
#include "ObjMgr.h"            //순서 안지키면 ObjMgr.h에서 에러남
#include "Monster_Melee.h"
#include "BmpMgr.h"
#include "PathFind.h"            //Find_NextStep() 사용을 위해
#include "ScrollMgr.h"
#include "SoundMgr.h"

CMonster_Melee::CMonster_Melee()
	:m_iAtk(0), m_bAlerted(false)
{

}
CMonster_Melee::~CMonster_Melee()
{

}

//Get_Distance  Find_NextStep  Is_Occupied  Get_Player
void CMonster_Melee::On_Turn() 
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player(); //플레이어의 정보를 가져와서 pPlayer에 저장
	
	if (!pPlayer)
		return;
	
	GRIDPOS playerPos = pPlayer->Get_GridPos(); //player에 설정되있는 좌표를 가져와서 playerPos에 저장 -> find_nextStep에 사용하기 위해
	GRIDPOS monsterPos = Get_GridPos();

	int iDistance = monsterPos.Get_Distance(playerPos);

	//몬스터의 시야거리보다 거리가 멀때
	if (iDistance > MONSTER_SIGHT_RANGE)
	{
		m_bAlerted = false;
		return;
	}
		
	if (!m_bAlerted)  //시야 범위 안에 플레이어가 처음으로 들어온 순간에만 얼럿 사운드 재생
	{
		m_bAlerted = true;
		Play_AlertSound();
	}
	

	if (iDistance == 1)  //플레이어와의 거리가 1이면 공격
	{
	   Attack(pPlayer); 	
	}
	else 
	{
		GRIDPOS next = CPathFind::Find_NextStep(monsterPos, playerPos); //플레이어의 좌표를 받아와서 플레이어와의 경로 계산
		Chase(next);
	}

	


}

void CMonster_Melee::Attack(CObj* _pTarget)
{
	_pTarget->Take_Damage(Get_Atk());

	//근접 몬스터가 플레이어를 때릴 때 공통 셰이크(보스는 자기 Attack()에서 더 큰 값으로 재호출해서 덮어씀)
	CScrollMgr::Get_Instance()->Start_Shake(3.f, 120);

	Play_AttackSound(); 
}

void CMonster_Melee::Take_Damage(int _dmg)
{
	CNewMonster::Take_Damage(_dmg);

	//근접 몬스터가 맞을 때 공통 셰이크(보스는 자기 Take_Damage()에서 더 큰 값으로 재호출해서 덮어씀)
	CScrollMgr::Get_Instance()->Start_Shake(2.f, 100);
}

void CMonster_Melee::Play_AttackSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_skill_killing_swing_startcast.wav"), CSoundMgr::MONSTER);
}

void CMonster_Melee::Play_AlertSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_human_alert_1.wav"), CSoundMgr::MONSTER);
}
