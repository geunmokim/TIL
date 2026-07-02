#pragma once
#include "NewMonster.h"
class CMonster_Melee : public CNewMonster
{
public:
	CMonster_Melee();
	virtual ~CMonster_Melee();

public:
	virtual void Initialize() override;
	virtual void On_Turn() override;

public:
	int Get_Atk() { return m_iAtk; }

private:
	//플레이어와의 거리가 1일시 플레이어를 공격
	void Attack(CObj* _pTarget);

	
	int m_iAtk;


};

