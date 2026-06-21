#pragma once
#include "Obj.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Set_Bullet(list<CObj*>*_pBullet) { m_pBullet = _pBullet; }
	void Set_Shield(list<CObj*>* _pShield) { m_pShield = _pShield; }
	

private:
	CObj* Create_Shield();
	
private:
	void Key_Check();
	void Jumping();

private:
	template <typename T>
	CObj* Create_Bullet()
	{
		return CAbFactory<T>::Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle);
	}

private:
	list<CObj*>* m_pBullet;  
	list<CObj*>* m_pShield;

	POINT m_tPosin;
	float m_fPosinDis;

	bool m_bJump;
	float m_fJumpPower;
	float m_fJumpTime;
	float m_fJumpY;

	

};

