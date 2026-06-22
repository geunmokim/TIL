#include "framework.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;
CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{

}

bool CKeyMgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}

	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
		m_bKeyState[_key] = !m_bKeyState[_key];

	return false;
}

bool CKeyMgr::Key_Up(int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}

	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x80000))
		m_bKeyState[_key] = !m_bKeyState[_key];

	return false;
}
