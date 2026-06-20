#pragma once

class CLine;
class CLineMgr
{

private:
	CLineMgr();
	~CLineMgr();

public:
	void Initialize();
	void Render(HDC _DC);
	void Release();

public:
	bool Collision_Line(float _x, float* _y);

public:
	static CLineMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CLineMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	list<CLine*> m_listLine;
	static CLineMgr* m_pInstance;
};

