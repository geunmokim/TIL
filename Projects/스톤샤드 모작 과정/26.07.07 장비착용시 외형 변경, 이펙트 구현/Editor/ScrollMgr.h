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
	static CScrollMgr* Get_Instance()
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
	static CScrollMgr* m_pInstance;

	float m_fScrollX;

};
