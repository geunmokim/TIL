#pragma once

#define VK_MAX 0xFF

class CKeyMgr
{

private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	static CKeyMgr* m_pInstance;

	bool m_bKeyState[VK_MAX];




};

