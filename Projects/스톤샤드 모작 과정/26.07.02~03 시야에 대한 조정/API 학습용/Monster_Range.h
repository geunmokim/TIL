#pragma once
#include "NewMonster.h"
class CMonster_Range :public CNewMonster
{
public:
	CMonster_Range();
	virtual ~CMonster_Range();

public:
	virtual void Initialize() override;
	virtual void On_Turn() override;

public:
	int Get_Atk() { return m_iAtk; }

private:
	void Attack(CObj* _pTarget);

private:
	int m_iAmmo;
	int m_iAttackRange;
	int m_iAtk;

};

