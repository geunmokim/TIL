#include "pch.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;
CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{

}
bool CKeyMgr::Key_Pressing(int _key)   //홀딩
{
	if (GetAsyncKeyState(_key) & 0x8000)  //계속 누른 상태라면 true를 반환
		return true;
	return false;                         //계속 누른 상태가 아니라면 false를 반환해서 false시 작동 중단
}

bool CKeyMgr::Key_Down(int _key)      //키 버튼이 다를떄가 아니라 키가 동작하는걸로 나뉨 //이건 키가 눌리는 순간을 감지
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

bool CKeyMgr::Key_Up(int _key)  //이건 키가 떼지는 순간을 감지
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}

	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
		m_bKeyState[_key] = !m_bKeyState[_key];

	return false;
}
