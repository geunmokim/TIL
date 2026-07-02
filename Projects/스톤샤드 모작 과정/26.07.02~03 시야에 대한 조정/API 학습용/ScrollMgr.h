#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float Get_ScrollX() { return m_fScrollX; }
	float Get_ScrollY() { return m_fScrollY; }

public:
	void Set_ScrollX(float _x) { m_fScrollX += _x; }
	void Set_ScrollY(float _y) { m_fScrollY += _y; }


	//목표 위치로 맞춰라는 의도를 표현하는 함수
	//고정값을 전달하는것 같지만 m_fScrollX라는 누적 변수를 갖고 있어서 나중에 다른 효과를 추가해도 자연스럽게 합쳐진다
	void Set_ScrollX_To(float _targetX) { m_fScrollX = _targetX; }
	void Set_ScrollY_To(float _targetY) { m_fScrollY = _targetY; }


public:
	void Scroll_Lock();
public:
	static CScrollMgr* Get_Instance() //static 붙여야만 싱글톤 패턴이 유지됨(객체 생성 없이 호출이 가능하니까)
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;
		return m_pInstance;

	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CScrollMgr* m_pInstance; //이 값이 static이여야만 위에서 Get_Instance()할때 어떤 m_pInstance인지 알 수 있음(static 성질에 의해 하나만 존재하니까)

	float m_fScrollX;
	float m_fScrollY;

};

