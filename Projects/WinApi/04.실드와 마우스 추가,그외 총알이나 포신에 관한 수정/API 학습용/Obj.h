#pragma once


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;    //= 0 의 의미 -> 순수 가상함수 // "나는 구현 안함, 자식이 반드시 구현해야 함" 
	virtual int Update() = 0;         //순수 가상함수 -> 자식 클래스가 반드시 구현하도록 강제하고, 부모 클래스는 직접 생성 못하게 막는 것
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;
public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_bDead() { m_bDead = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }

protected:
	void Update_Rect();   //모든 오브젝트가 공통적으로 사용하는 함수이다 
	                      //그럼 왜 virtual로 상속 안할까? -> 모든 오브젝트가 똑같은 방식으로 Rect를 계산하기 때문 
	                      //virtual이 필요한 경우는  오브젝트마다 동작이 다를 경우 -> 동작이 같으면 일반 함수, 다르면 virtual

	/*
	Update_Rect()를 obj의 자식들에게 상속시키는 이유는
	자식들이 랜더링 할떄 Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);이런식으로
	m_tRect의 값을 가져다 쓰기 때문 또한 충돌 판정에서도 
	if (IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect())) //&rc-> 겹친 영역을 저장할 변수,첫번쨰rect,두번쨰rect //-> 겹치면 true반환
	이런식으로 rect를 가져다 쓰기때문에
	
1. 렌더링용
   Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom)
   → 오브젝트가 이동하면 그려지는 사각형도 따라다녀야 함

2. 충돌 판정용 (CollisionMgr)
   IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect())
   → 정확한 위치에서 충돌 판정이 이루어져야 함
   의 두가지 이유로 상속시키고 상속받는 클래스들이 update()마다 update_rect()를 해줘야함
	
	*/
protected:
	INFO m_tInfo;
	RECT m_tRect;
	CObj* m_pTarget;

	float m_fSpeed;
	float m_fAngle;
	float m_fDis;

	bool m_bDead;


};

