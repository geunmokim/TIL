#include "framework.h"
#include "Button.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CMyButton::CMyButton()
{

}

CMyButton::~CMyButton()
{
	Release();
}


void CMyButton::Initialize()
{
	m_tInfo.iCX = 150;  //버튼 하나의 사진 크기
	m_tInfo.iCY = 150;  //150,150
}

int CMyButton::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT pt = {};
	GetCursorPos(&pt);             //커서를 화면으로 가져오는 내장 함수
	ScreenToClient(g_hWnd, &pt);   //GetCursorPos와 세트


	//마우스의 좌표가 이미지의 좌표와 겹칠때 //// 마우스가 버튼 위에 있다면
	if (PtInRect(&m_tRect, pt))  //BOOL PtInRect(const RECT* lprc, POINT pt); -> 어떤 점(POINT)이 특정 사각형(RECT) 영역 안에 들어있는지 검사해주는 함수
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))  //마우스가 버튼 위에 있고 마우스 왼쪽 버튼 눌림 감지했을때
		{
			if (!lstrcmp(m_pImageKey, L"Start"))     //L"Start"로 유니코드를 사용한다고 못박았지만 사실 _T"Strat"가 더 유연하게 사용할 수 있어서 좋다
			{                                       //_T"Start"는 auto처럼 자동으로 프로젝트 설정에 맞춰 바꿔주기 때문
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::STAGE);
			}
			else if (!lstrcmp(m_pImageKey, L"Edit"))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::EDIT);
			}
			else if (!lstrcmp(m_pImageKey, L"Exit"))
			{
				DestroyWindow(g_hWnd);
				return;
			}
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);

	GdiTransparentBlt(_DC            //도화지
		, m_tRect.left, m_tRect.top  //시작 좌표
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_iDrawID * 150, 0
		, 150, 150                   //사진 크기
		, RGB(255, 255, 255));       //색깔
}

void CMyButton::Release()
{

}