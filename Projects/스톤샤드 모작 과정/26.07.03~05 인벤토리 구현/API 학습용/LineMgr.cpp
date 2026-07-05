#include "framework.h"
#include "LineMgr.h"
#include "Line.h"

CLineMgr* CLineMgr::m_pInstance = nullptr;
CLineMgr::CLineMgr()
{

}

CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	Load_Line();
	/*LINEPOS tPos[4] = { {100.f, 500.f}, {300.f, 500.f},{500,300.f},{700.f,300.f} };

	m_listLine.emplace_back(new CLine(tPos[0], tPos[1]));
	m_listLine.emplace_back(new CLine(tPos[1], tPos[2]));
	m_listLine.emplace_back(new CLine(tPos[2], tPos[3]));*/
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

bool CLineMgr::Collision_Line(float _x, float* _y)
{
	if (m_listLine.empty())
		return false;

	for (auto& pLine : m_listLine)
	{
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& pLine->Get_Info().tRightPos.fX > _x) //끝부분을 =시키지 않는 이유는 선이 이어지기 때문 
			                                        //구간을 겹치지 않게 나누는 수학/프로그래밍의 일반적인 기법
		{
			float	x1 = pLine->Get_Info().tLeftPos.fX;
			float	y1 = pLine->Get_Info().tLeftPos.fY;
			float	x2 = pLine->Get_Info().tRightPos.fX;
			float	y2 = pLine->Get_Info().tRightPos.fY;
			
			//*y를 사용하는 이유는 c++에선 함수가 하나의 값만을 return한다 -> 지금 bool이기 떄문에 return true;로 true;를 내보내는중
			//충돌하는 순간의 y좌표를 알고 싶으니 이런 식으로 포인터를 이용해 간접적으로 값을 내보내는것
			*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;
			return true;
		}
	}

	return false;
}

void CLineMgr::Load_Line()
{
	HANDLE hFile = CreateFile(L"../Data/Line.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	LINEINFO	tTemp;
	DWORD		dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, NULL);

		if (0 == dwByte)
			break;

		m_listLine.emplace_back(new CLine(tTemp.tLeftPos, tTemp.tRightPos));
	}

	CloseHandle(hFile);
}
