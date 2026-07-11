#pragma once
#include "Monster_Melee.h"
class CBandit_Cleaver : public CMonster_Melee
{
public:
	CBandit_Cleaver();
	virtual ~CBandit_Cleaver();

public:
	virtual void Initialize() override;
};
