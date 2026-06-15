#pragma once
#include "Obj.h"

class CMonster : public CObj
{
public:
	CMonster();
	CMonster(const INFO& _rInfo);//(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel); 구버전에선 이렇게 나열해서 썼지만 지금은 struct에서 INFO에 관해 오버라이딩으로 정리해놨음
	~CMonster();
public:
	virtual void Initialize();
	virtual void Render();
	virtual void Release();

public:
	int Get_Exp();
};

