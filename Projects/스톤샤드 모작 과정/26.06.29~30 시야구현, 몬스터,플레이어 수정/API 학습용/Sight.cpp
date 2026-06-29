#include "framework.h"
#include "Sight.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "ScrollMgr.h"
#pragma comment(lib, "Msimg32.lib") //alphablend 사용하기 위해 추가

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
	

	for (int i = 0; i < TILEY; ++i)          //이 식을 통해 타일과 같은 인덱스를 공유하게 만들어 타일에 상태를 부여한다
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int index = i * TILEX + j;     //마찬가지로 이 식이 있어야 m_vecTile과 같은 인덱스를 공유하게 된다
			GRIDPOS TilePos(i, j);
			
			int iDistance = TilePos.Get_Distance(playerPos); //타일 하나하나를 플레이어의 좌표와 비교해 거리 계산

			if (iDistance <= PLAYER_SIGHT_RANGE) //거리가 플레이어의 시야거리 안쪽이라면
			{
				m_vecVisibility[index] = VISIBLE; //해당 타일 인덱스를 visible로
			
			}
			else //거리밖일떄
			{
				if (m_vecVisibility[index] == VISIBLE) //이미 탐험한 곳이면 
				{
					m_vecVisibility[index] = EXPLORED; 
				}
				
				
			}
				
			
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
