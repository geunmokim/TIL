#pragma once
#include "Monster_Melee.h"

class CAncientTroll : public CMonster_Melee
{
public:
	CAncientTroll();
	virtual ~CAncientTroll();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render(HDC _DC) override;

private:
	virtual void Attack(CObj* _pTarget) override;
	virtual void Take_Damage(int _dmg) override;
};
