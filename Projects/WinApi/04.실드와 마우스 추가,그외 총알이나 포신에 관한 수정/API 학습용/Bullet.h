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
	//void My_AsyncKeyState(int _Mykey) { m_Key = _Mykey; }
	void Set_Dir(BULLET::DIR _eDir) { m_eDir = _eDir; }
	/*void Set_MyAngle(float _fMyAngle) { m_fAngle = _fMyAngle; }*/

private:
	//int m_Key;
	BULLET::DIR		m_eDir;
	//namespace로 enum인 BULLET이 선언되었기에 BULLET::DIR m_eDir 형식이 됨
	//namespace로 BULLET이 만들어 지지 않고 그냥 enum BULLET_DIR { B_LEFT, B_RIGHT, B_UP, B_DOWN, B_END };식이라면?
	//->그럼 BULLET_DIR m_eDir로 _만 붙이면 enum BULLET을 호출할 수 있다
	

};

