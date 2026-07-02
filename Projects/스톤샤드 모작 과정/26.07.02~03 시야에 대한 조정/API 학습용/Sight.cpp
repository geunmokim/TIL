#include "framework.h"
#include "Sight.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#pragma comment(lib, "Msimg32.lib") //alphablend 사용하기 위해 추가
#include "TileMgr.h"
#include <queue>

CSight*CSight:: m_pInstance = nullptr;


CSight::CSight()
	:m_iSize(TILEX* TILEY)
{
	m_vecVisibility.reserve(m_iSize);
}
CSight::~CSight()
{
	Release();
}


void CSight::Initialize()
{
	m_vecVisibility.resize(m_iSize,UNSEEN); //일단은 600의 값을 가지는 벡터를 UNSEEN(0)으로 채워넣는다
	

}

void CSight::Update_Sight()
{
	// == GRIDPOS playerPos = CObjMgr::Get_Instance()->Get_Player()->Get_GridPos();
	CObj* Player = CObjMgr::Get_Instance()->Get_Player(); //플레이어의 정보를 가져와서
	GRIDPOS playerPos = Player->Get_GridPos();              //플레이어의 좌표를 가져온다
	
	//일차적으로 기존 visible을 explore 상태로 만들어주고 밑에서 bfs로 현재 방에 대해 visible로 다시 바꿔준다
	for (int i = 0; i < m_iSize; ++i)
	{
		if (m_vecVisibility[i] == VISIBLE)
			m_vecVisibility[i] = EXPLORED;
	}

	//bfs로 플레이어 위치에서 벽을 넘지 않고 연결된 바닥 타일 전체를 visible로 설정
	vector<bool> visited(m_iSize, false);
	queue<pair<GRIDPOS, int>> q;  // GRIDPOS + 현재 거리
	q.push({ playerPos, 0 });
	visited[playerPos.row * TILEX + playerPos.col] = true;
	m_vecVisibility[playerPos.row * TILEX + playerPos.col] = VISIBLE;

	while (!q.empty())
	{
		GRIDPOS cur = q.front().first;
		int dist = q.front().second;
		q.pop();

		// 거리 제한 초과 시 더 이상 탐색 안 함
		if (dist >= PLAYER_SIGHT_RANGE)
			continue;



		// 8방향 탐색
		int dr[] = { -1,-1,-1, 0, 0, 1, 1, 1 };
		int dc[] = { -1, 0, 1,-1, 1,-1, 0, 1 };

		for (int d = 0; d < 8; ++d)
		{
			int nr = cur.row + dr[d];
			int nc = cur.col + dc[d];

			if (nr < 0 || nr >= TILEY || nc < 0 || nc >= TILEX)
				continue;

			int idx = nr * TILEX + nc;
			if (visited[idx])
				continue;

			visited[idx] = true;

			// 벽이면 EXPLORED로만 표시하고 더 이상 탐색 안 함
			if (CTileMgr::Get_Instance()->Is_Wall(nr, nc))
			{
				if (m_vecVisibility[idx] != VISIBLE)
					m_vecVisibility[idx] = EXPLORED;
				visited[idx] = true;
				continue;
			}

			m_vecVisibility[idx] = VISIBLE;
			q.push({ GRIDPOS(nr, nc), dist + 1 });
		}
	}
}

void CSight::Render(HDC _DC)
{

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int index = i * TILEX + j;

			if (VISIBLE == m_vecVisibility[index])
				continue;

			// 칸 번호(i,j)를 화면 픽셀 좌표로 변환 (스크롤 보정 포함)
			int iX = j * TILECX + iScrollX;
			int iY = i * TILECY + iScrollY;
			RECT tRect = { iX, iY, iX + TILECX, iY + TILECY };

			if (m_vecVisibility[index] == UNSEEN)
			{
				HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				FillRect(_DC, &tRect, hBlackBrush);
			}
			else if (m_vecVisibility[index] == EXPLORED)
			{
				// GDI의 FillRect는 반투명을 못 만들기 때문에, AlphaBlend로 처리한다.
				// 1. 임시 메모리 화면을 만들어 거기에 검은 사각형을 그려둠
				HDC hMemDC = CreateCompatibleDC(_DC);
				HBITMAP hBitmap = CreateCompatibleBitmap(_DC, TILECX, TILECY);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

				RECT tLocalRect = { 0, 0, TILECX, TILECY };
				HBRUSH hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				FillRect(hMemDC, &tLocalRect, hBlackBrush);

				// 2. 그 임시 화면을 실제 화면 위에 반투명하게 섞어서 그림
				BLENDFUNCTION tBlend = {};
				tBlend.BlendOp = AC_SRC_OVER;
				tBlend.SourceConstantAlpha = 120;	// 0(완전투명)~255(완전불투명)

				AlphaBlend(_DC, iX, iY, TILECX, TILECY
					, hMemDC, 0, 0, TILECX, TILECY
					, tBlend);

				// 3. 임시로 만들었던 자원은 반드시 정리
				SelectObject(hMemDC, hOldBitmap);
				DeleteObject(hBitmap);
				DeleteDC(hMemDC);


			}

		}



	}
}










void CSight::Release()
{
	m_vecVisibility.clear();
}
