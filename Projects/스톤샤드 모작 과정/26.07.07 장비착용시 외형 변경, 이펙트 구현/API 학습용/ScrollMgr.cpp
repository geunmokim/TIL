#include "framework.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;
CScrollMgr::CScrollMgr()
	:m_fScrollX(0.f),m_fScrollY(0.f)
{

}

CScrollMgr::~CScrollMgr()
{

}


void CScrollMgr::Scroll_Lock()
{
	if (0 <= m_fScrollX)
		m_fScrollX = 0.f;
	if (WINCX - (TILEX * TILECX) >= m_fScrollX)  //TILEX * TILECX = 맵전체의 가로길이 
		m_fScrollX = WINCX - (TILEX * TILECX);  //화면보다 맵이 더 넓을 때, 맵의 오른쪽 끝이 화면 오른쪽에 딱 맞게 보이는 시점의 ScrollX 값을 구하는 식

	if (0 <= m_fScrollY)
		m_fScrollY = 0.f;
	if (WINCY - (TILEY * TILECY) >= m_fScrollY)
		m_fScrollY = WINCY - (TILEY * TILECY);
}

/*
맵은 크기가 정해져 있고(한도가 있고), 
플레이어가 계속 한쪽 방향으로 이동하면 스크롤도 끝없이 따라 움직이려 하니,
결국 언젠가는 "맵 데이터가 끝나는 지점"을 넘어서서 아무것도 없는 빈 공간까지 화면에 보이게 될 수밖에 없는데,
Scroll_Lock()이 바로 그걸 막아주는 안전장치
*/