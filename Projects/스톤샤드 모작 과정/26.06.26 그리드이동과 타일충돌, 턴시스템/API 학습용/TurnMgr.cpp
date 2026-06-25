#include "framework.h"
#include "TurnMgr.h"
#include "ObjMgr.h"
#include "Obj.h"

CTurnMgr* CTurnMgr::m_pInstance = nullptr;

CTurnMgr::CTurnMgr()
	:m_eTurnState(PLAYER_TURN)
{

}

CTurnMgr::~CTurnMgr()
{
	Release();
}

void CTurnMgr::Initialize()
{
	m_eTurnState = PLAYER_TURN;

}

void CTurnMgr::Update()
{
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
		if (pMonster->Is_Dead())
			continue;

		pMonster->On_Turn();
	}

	m_eTurnState = PLAYER_TURN;
}