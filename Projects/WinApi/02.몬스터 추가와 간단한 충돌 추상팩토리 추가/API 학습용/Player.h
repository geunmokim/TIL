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
	//void My_AsyncKeyState(int _Mykey) { m_Key = _Mykey; }


private:
	CObj* Create_Bullet(BULLET::DIR _eDir);


private:
	list<CObj*>* m_pBullet;  //list로 만든 이유는 총알을 한개만 만들게 아니라 여러개 만들거기 때문 
	                         //vector, map도 있는데 왜 list? -> 총알은 삭제할 일이 많기 때문에(사용하면 삭제해야하니까)
	                         //그럼 vector는 언제? -> 타일이나 웨이포인트 애니메이션 프레임 등등 삭제할일이 없는 경우에 사용

	
/*
list<CBullet*>* 대신 list<CObj*>*를 사용하는 이유 -> 다형성 때문에
총알 종류가 늘어날 경우
이렇게 하면 총알 종류마다 리스트를 따로 만들어야 함
list<CBullet*>*     m_pBullet;
list<CFireBullet*>* m_pFireBullet;   // 불 총알
list<CIceBullet*>*  m_pIceBullet;    // 얼음 총알

 // CObj*로 하면 하나로 다 관리 가능
list<CObj*>* m_pBullet;

m_pBullet->emplace_back(new CBullet());
m_pBullet->emplace_back(new CFireBullet());  // 그냥 추가하면 됨
m_pBullet->emplace_back(new CIceBullet());   // 그냥 추가하면 됨
*/
	int m_Key;

};

