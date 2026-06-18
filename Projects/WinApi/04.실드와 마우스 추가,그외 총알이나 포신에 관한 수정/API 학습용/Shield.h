#pragma once
#include "Obj.h"
class CShield : public CObj
{
public:              
	CShield();           //생성자는 public 선언 안해주면 기본 private 설정이다
	virtual ~CShield();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC)override;
	virtual void Release() override;

};

