#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float Get_ScrollX() { return m_fScrollX; }

public:
	void Set_ScrollX(float _x) { m_fScrollX += _x; }
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

};

