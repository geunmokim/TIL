#pragma once
#include "NewMonster.h"
class CMonster_Melee : public CNewMonster
{
public:
	CMonster_Melee();
	virtual ~CMonster_Melee();

public:
	virtual void On_Turn() override;

public:
	int Get_Atk() { return m_iAtk; }

protected:
	//플레이어와의 거리가 1일시 플레이어를 공격
	//보스몬스터가 attack에 start_shake를 넣어야해서 protected, virtual로 변경
	virtual void Attack(CObj* _pTarget);
	virtual void Take_Damage(int _dmg) override;
protected:
	int m_iAtk;


};

