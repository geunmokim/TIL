#pragma once
#include "Obj.h"
class CFont : public CObj
{
public:
	CFont();
	CFont(int _iDamage, const GRIDPOS& _targetPos);
	virtual ~CFont();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;



private:
	int m_iLifeFrame; // 표시 지속 프레임
	int m_iDamage; //표시할 데미지 숫자

	GRIDPOS m_tTargetPos;
};

