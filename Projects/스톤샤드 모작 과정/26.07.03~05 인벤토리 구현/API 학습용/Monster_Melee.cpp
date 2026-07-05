#include "framework.h"
#include "ObjMgr.h"            //순서 안지키면 ObjMgr.h에서 에러남
#include "Monster_Melee.h"
#include "BmpMgr.h"
#include "PathFind.h"            //Find_NextStep() 사용을 위해


CMonster_Melee::CMonster_Melee()
	:m_iAtk(0)
{

}
CMonster_Melee::~CMonster_Melee()
{

}

void CMonster_Melee::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/s_bandit_goon_cleaver01_0.bmp"), _T("Bandit_Cleaver"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	m_iSrcCX = 28;  //BMP의 크기
	m_iSrcCY = 34;  //BMP의 크기

	Set_GridPos(8, 8); // 시작 칸 위치 //일단 임의로 잡아둔 값
	m_iHp = 45;
	m_iAtk = 10;

	m_pImageKey = _T("Bandit_Cleaver");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
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
		return;

	

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

	
}


