#include "framework.h"
#include "TurnMgr.h"
#include "ObjMgr.h"
#include "Obj.h"

CTurnMgr* CTurnMgr::m_pInstance = nullptr;

CTurnMgr::CTurnMgr()
	:m_eTurnState(PLAYER_TURN)
	,m_bGameOver(false)
{

}

CTurnMgr::~CTurnMgr()
{
	Release();
}

void CTurnMgr::Initialize()
{
	m_eTurnState = PLAYER_TURN;
	m_bGameOver = false;

}

void CTurnMgr::Update()
{
	//플레이어 사망 상태면, 더이상 턴을 진행하지 않는다
	if (m_bGameOver)
		return;



	if (ENEMY_TURN == m_eTurnState)
		Process_EnemyTurn();
}

void CTurnMgr::Release()
{

}

void CTurnMgr::End_PlayerTurn()
{
	m_eTurnState = ENEMY_TURN;
}

void CTurnMgr::Process_EnemyTurn()
{
	const list<CObj*>& listMonster = CObjMgr::Get_Instance()->Get_MonsterList();

	for (auto& pMonster : listMonster)
	{
		if (pMonster->Is_Dead())   //죽은 상태라면 턴을 가지지 못하게
			continue;

		pMonster->On_Turn();
	}

	// 몬스터들의 공격으로 이번 적 턴 중에 플레이어가 죽었을 수 있으므로,
	// 다시 PLAYER_TURN으로 돌아가기 전에 반드시 여기서 확인한다.
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayer && pPlayer->Is_Dead())
	{
		m_bGameOver = true;
		return;
	}
	m_eTurnState = PLAYER_TURN;
}