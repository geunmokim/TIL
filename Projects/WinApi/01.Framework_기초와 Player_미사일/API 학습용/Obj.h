#pragma once


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;
public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }

protected:
	void Update_Rect();   //모든 오브젝트가 공통적으로 사용하는 함수이다 
	                      //그럼 왜 virtual로 상속 안할까? -> 모든 오브젝트가 똑같은 방식으로 Rect를 계산하기 때문 
	                      //virtual이 필요한 경우는  오브젝트마다 동작이 다를 경우 -> 동작이 같으면 일반 함수, 다르면 virtual
protected:
	INFO m_tInfo;
	RECT m_tRect;

	float m_fSpeed;

};

