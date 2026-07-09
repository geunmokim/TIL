#pragma once
#include "Monster_Melee.h"
enum GHOUL_SIZE { GHOUL_SMALL, GHOUL_LARGE };

class CGhoul : public CMonster_Melee
{
public:
	CGhoul();
	CGhoul(GHOUL_SIZE _eSize);
	virtual ~CGhoul();

public:
	virtual void Initialize() override;			//CAbFactory 등 기본 호출용 (미사용시 Small 기준으로 초기화)
	void Initialize(GHOUL_SIZE _eSize);			//실제로 사용할 초기화 함수, Small/Large 구분해서 스탯/스프라이트 세팅

protected:
	virtual void Play_AlertSound() override;
	virtual void Play_DeathSound() override;
private:
	GHOUL_SIZE m_eSize;
};