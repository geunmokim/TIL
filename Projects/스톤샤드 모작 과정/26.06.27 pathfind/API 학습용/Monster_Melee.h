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

};

