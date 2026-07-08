#include "pch.h"
#include "LineMgr.h"
#include "Line.h"
#include "ScrollMgr.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;
CLineMgr::CLineMgr()
	:m_bLineCheck(false)
{

}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	m_bLineCheck = true;
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& pLine : m_listLine)
		pLine->Render(_DC);

}

void CLineMgr::Release()
{
	for_each(m_listLine.begin(), m_listLine.end(), Safe_Delete<CLine*>);
	m_listLine.clear();
}

void CLineMgr::Create_Line()
{
	POINT pt = {};
	GetCursorPos(&pt);                //마우스의 모니터 전체 기준 좌표
	ScreenToClient(g_hWnd, &pt);      // 그 좌표를 내 창 내부 기준 좌표로 변환 //보통 GetCursorPos와 한쌍으로 사용됨
	

	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX(); //화면 좌표를 맵(월드) 좌표로 다시 한 번 변환하는 작업
	                                                        //ScrollX를 빼줘야 비로소 맵 전체 기준으로 진짜 어디에 그렸는지가 정확히 데이터에 저장
	if (m_bLineCheck)
	{
		m_tLinePos[0].fX = (float)pt.x;
		m_tLinePos[0].fY = (float)pt.y;

		m_bLineCheck = false;
	}
	else
	{
		m_tLinePos[1].fX = (float)pt.x;
		m_tLinePos[1].fY = (float)pt.y;

		m_listLine.emplace_back(new CLine(m_tLinePos[0], m_tLinePos[1]));

		m_tLinePos[0] = m_tLinePos[1];

	}

}
void CLineMgr::Save_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패!", L"실패", MB_OK);
		return;
	}

	DWORD	dwByte = 0;

	for (auto& pLine : m_listLine)
		WriteFile(hFile, &pLine->Get_Info(), sizeof(LINEINFO), &dwByte, NULL);

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공!", L"성공", MB_OK);
}

void CLineMgr::Load_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패!", L"실패", MB_OK);
		return;
	}

	Release();

	DWORD		dwByte = 0;
	LINEINFO	tTemp;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if (0 == dwByte)
			break;

		m_listLine.emplace_back(new CLine(tTemp.tLeftPos, tTemp.tRightPos));

	}
	MessageBox(g_hWnd, L"불러오기 성공!", L"성공", MB_OK);
	CloseHandle(hFile);
}


bool CLineMgr::Collision_Line(float _x, float* _y)
{
	if (m_listLine.empty())
		return false;

	for (auto& pLine : m_listLine)
	{
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& pLine->Get_Info().tRightPos.fX > _x)
		{
			float	x1 = pLine->Get_Info().tLeftPos.fX;
			float	y1 = pLine->Get_Info().tLeftPos.fY;
			float	x2 = pLine->Get_Info().tRightPos.fX;
			float	y2 = pLine->Get_Info().tRightPos.fY;

			*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;
			return true;
		}
	}

	return false;
}