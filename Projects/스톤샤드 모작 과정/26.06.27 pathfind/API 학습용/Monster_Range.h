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

private:
	int m_iAmmo;

};

