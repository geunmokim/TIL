#pragma once
#include "Obj.h"
class CNewMonster :public CObj
{
	//Monster_Melee와 Monster_Range의 공통부분을 작성하는 Monster들의 부모
public:
	CNewMonster();
	virtual ~CNewMonster();

public:
	virtual void Initialize() override = 0;  //melee와 range에서 다른 스프라이트를 사용하기 때문에 이부분은 공통부분이 아니라서
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC)override;
	virtual void Release() override;
	virtual void On_Turn() override = 0;


protected:
	//플레이어를 발견시 추격
	void Chase(const GRIDPOS& _next);

protected:
	int m_iSrcCX;
	int m_iSrcCY;

private:
	int m_iAtk;
};

