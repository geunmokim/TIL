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

	virtual void Take_Damage(int _dmg) override;

	virtual void On_Turn() override;         	//사망 애니메이션 중 턴 행동 못하도록


private:
	virtual void Attack(CObj* _pTarget) override;
	virtual void Play_AlertSound() override;
	virtual void Play_AttackSound() override;
	virtual void Play_DeathSound() override;

private:
	//공격 애니메이션용 //idle과 별도 상태로 관리 (공격 떄 한번만 재생 후 idle로 복귀)
	bool m_bIsAttacking;
	FRAME m_tAttackFrame;
	int m_iAttackSrcCX;
	int m_iAttackSrcCY;


	//사망 애니메이션용  //다 재생될때까지 진짜 죽음(m_bDead) 처리를 미룸
	bool m_bIsDying;
	FRAME m_tDeadFrame;
	int m_iDeadSrcCX;
	int m_iDeadSrcCY;

};

