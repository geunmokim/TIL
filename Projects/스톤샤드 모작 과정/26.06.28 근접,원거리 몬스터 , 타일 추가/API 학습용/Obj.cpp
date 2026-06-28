#include "framework.h"
#include "Obj.h"
CObj::CObj()
	:m_fSpeed(0.f), m_bDead(false),m_fAngle(0.f), m_fDis(0.f), m_pTarget(nullptr)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo)); //구조체 멤버변수는 초기화 안하면 쓰레기 값이 들어가기 때문에 생성자에서 zeromemory를 통해 0으로 초기화
	ZeroMemory(&m_tRect, sizeof(m_tRect)); //위와 같은 이유
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CObj::~CObj()
{

}
void CObj::Update_Rect() //Rect() = Rectangle()
{
	m_tRect.left = (LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1));    //사각형의 왼쪽 X 좌표
	m_tRect.right = (LONG)(m_tInfo.fX + (m_tInfo.iCX >> 1));    //사각형의 오른쪽 X좌표
	m_tRect.top = (LONG)(m_tInfo.fY - (m_tInfo.iCY >> 1));      //사각형의 왼쪽 Y좌표
	m_tRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCY >> 1));  //사각형의 오른쪽 Y좌표

}
void CObj::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwDelayTime < GetTickCount())
	{
		++m_tFrame.iStartX;

		if (m_tFrame.iStartX >= m_tFrame.iEndX)
			m_tFrame.iStartX = 0;

		m_tFrame.dwTime = GetTickCount();

	}
}