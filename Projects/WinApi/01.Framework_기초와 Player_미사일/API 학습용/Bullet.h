#pragma once
#include "Obj.h"


class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void My_AsyncKeyState(int _Mykey) { m_Key = _Mykey; }

private:
	int m_Key;
};

