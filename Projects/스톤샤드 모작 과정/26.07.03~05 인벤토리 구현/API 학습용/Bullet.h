#pragma once
#include "Obj.h"


class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:

	void Set_Dir(BULLET::DIR _eDir) { m_eDir = _eDir; }


private:

	BULLET::DIR		m_eDir;

	

};

