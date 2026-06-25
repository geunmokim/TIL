#include "framework.h"
#include "TileMgr.h"
#include "Tile.h"
#include "SCrollMgr.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;
CTileMgr::CTileMgr()
	:m_iSize(TILEX* TILEY)
{
	m_vecTile.reserve(m_iSize);

}



CTileMgr::~CTileMgr()
{
	Release();
}
//타일을 필요한 부분만 그릴 때 소수점 자리 때문에 화면이 짤려 보일 수 있다 
// ->+2을 하므로써 화면이 짤려보이는 것을 막을 수 있다(+1은 백버퍼로 움직일때 부자연스럽게 추가됨)

//y값에 상관없는 것들 --- ui,타일

//그리는 순서는 배경, 오브젝트,이펙트, UI순으로 그리자 //이중 오브젝트만 Y값을 비교한다
void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)          //가로로 긴 맵을 만들고 싶어서 TILEY를 넣음 //TILEX를 넣는다면 세로로 긴 맵이 형성됨
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)((TILECX >> 1) + (j * TILECX));   //타일의 중심 좌표를 구하는 식
			float fY = (float)((TILECY >> 1) + (i * TILECY));  //타일의 중심 좌표를 구하는 식

			CObj* pObj = CAbFactory<CTile>::Create(fX, fY);
			m_vecTile.emplace_back(pObj);

		}
	}
}

void CTileMgr::Render(HDC _DC)
{
	int		iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int		iCullY = 0;

	int		iCullEndX = iCullX + (WINCX / TILECX) + 2;//화면 경계에서 타일이 살짝 잘려 보일 경우를 대비해서 +2로 한두칸 더 여유있게 그리게함
	int		iCullEndY = iCullY + (WINCY / TILECY) + 2;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			
			int iIdx = i * TILEX + j;
			//위에서 사용된 +2 즉, 여유분 떄문에 실제 던전크기를 넘어가는 계산이 나올 수 있으므로 그리기전에 범위 체크
			if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
				continue;

			m_vecTile[iIdx]->Render(_DC);
		}
	}

}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObj*>);
	m_vecTile.clear();

}



void CTileMgr::Picking_Tile(int _iDrawID)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);                           //GetCursorPos/ScreenToClient로 얻은 pt.x는 화면에 보이는 픽셀 위치(스크린 좌표)
	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();//월드 좌표 = 화면좌표 - ScrollX (이건 내가 ScrollX를 어떻게 설정하냐에 따라 +,-가 바뀜)
	                                                       //현재 // 오른쪽 이동 → ScrollX는 - 때문에 -가 된것
	int x = pt.x / TILECX;    //마우스가 몇번쨰 타일에 있는지
	int y = pt.y / TILECY;
	int iIdx = y * TILEX + x; //인덱스 구하는 공식


	/*
	
	      픽셀 좌표 vs 타일 인덱스
          타일 0      타일 1      타일 2      타일 3
          ┌────────┬────────┬────────┬────────┐
          0        64       128      192      256   ← 픽셀 좌표 (px)
          0         1         2         3            ← 타일 인덱스
	
	
	*/

	if (0 > iIdx || m_iSize <= (size_t)iIdx)
		return;
	m_vecTile[iIdx]->Set_DrawID(_iDrawID);
}

bool CTileMgr::Is_Wall(int _row, int _col)
{
	// Picking_Tile()과 같은 방식으로, (row, col)을 1차원 vector 인덱스로 변환한다.
	int iIdx = _row * TILEX + _col;

	//벽 판단전에 범위 체크
	if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
		return true;

	//DrawId가 WALL_DRRAWID 이상이면 벽 취급
	return m_vecTile[iIdx]->Get_DrawID() >= WALL_DRAWID;
}






void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;

	for (auto& pTile : m_vecTile)
	{
		WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &pTile->Get_DrawID(), sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	Release();

	DWORD	dwByte = 0;
	INFO	tTemp = {};
	int		iDrawID = 0;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, NULL);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbFactory<CTile>::Create(tTemp.fX, tTemp.fY);
		pObj->Set_DrawID(iDrawID);
		m_vecTile.emplace_back(pObj);
	}

	CloseHandle(hFile);
}
