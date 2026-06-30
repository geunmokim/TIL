#include "framework.h"
#include "NewPlayer.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "TurnMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Sight.h"
CNewPlayer::CNewPlayer()
	:m_bKeyLock(false),m_iAtk(0)
{

}

CNewPlayer::~CNewPlayer()
{
	Release();
}

void CNewPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Player/s_player_0.bmp"), _T("PlayerWalk"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;
	
	Set_GridPos(5, 5); // 시작 칸 위치. Set_GridPos가 m_tInfo.fX/fY까지 같이 맞춰준다
	m_iHp = 100;

	m_iAtk = 15;

	m_pImageKey = _T("PlayerWalk");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
	m_bKeyLock = false;

}
int CNewPlayer::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check();

	Update_Rect();

	return OBJ_NOEVENT;
}

void CNewPlayer::Late_Update()
{
}
void CNewPlayer::Render(HDC _DC)
{
	Update_Rect();

	HDC hPlayerDC = CBmpMgr::Get_Instance()->Find_Image(m_pImageKey);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//화면 어디에 그릴지를 계산하기 위해 스크롤값을 더해준다
	GdiTransparentBlt(_DC,
		m_tRect.left + iScrollX, m_tRect.top + iScrollY,
		48, 40,
		hPlayerDC,
		0, 0,
		48, 40,
		RGB(255, 255, 255));


}
void CNewPlayer::Release()
{

}

bool CNewPlayer::Can_Move(int _row, int _col)
{
	//던전 범위를 벗어나면 이동 불가

	if (_row < 0 || _row >= GRID_ROW_MAX) 
		return false;
	if (_col < 0 || _col >= GRID_COL_MAX)
		return false;

	//범위 안이라도 그 칸이 벽이라면 이동 불가(DrawaID 기준으로 판단)
	if (CTileMgr::Get_Instance()->Is_Wall(_row, _col))
		return false;

	// 그 칸에 살아있는 몬스터가 있다면, 겹쳐서 이동할 수 없다.
	if (CObjMgr::Get_Instance()->Is_Occupied(GRIDPOS(_row, _col)))
		return false;

	return true;
}

void CNewPlayer::Key_Check()
{
	//플레이어 사망상태면 더이상 입력을 받지 않는다
	if (CTurnMgr::Get_Instance()->Is_GameOver())
		return;

	//플레이어 턴이 아니라면 키를 입력해도 무효화
	if (PLAYER_TURN != CTurnMgr::Get_Instance()->Get_TurnState())
		return;


	int row = m_tGridPos.row;
	int col = m_tGridPos.col;

	//대각선 이동을 위해 각 키의 상태를 따로 얻어 둔다(Key_Down은 순간만 잡아내서 대신 Key_Pressing 사용)
	bool bLeft = CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT);
	bool bRight = CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT);
	bool bUp = CKeyMgr::Get_Instance()->Key_Pressing(VK_UP);
	bool bDown = CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN);


	//상하좌우 모든 방향키가 떨어졌다면 잠금을 해제
	if (!bLeft && !bRight && !bUp && !bDown)
		m_bKeyLock = false;

	if (m_bKeyLock)
		return;

	if (bLeft && bUp)
	{
		col -= 1; row -= 1;
	}
	else if (bLeft && bDown)
	{
		col -= 1; row += 1;
	}
	else if (bRight && bUp)
	{
		col += 1; row -= 1;
	}
	else if (bRight && bDown)
	{
		col += 1; row += 1;
	}
	else if (bLeft)
	{
		col -= 1;
	}
	else if (bRight)
	{
		col += 1;
	}
	else if (bUp)
	{
		row -= 1;
	}
	else if (bDown)
	{
		row += 1;
	}
	else
		return;



	if (Can_Move(row, col))
	{
		Set_GridPos(row, col);

		m_bKeyLock = true;

		CSight::Get_Instance()->Update_Sight();

		float fTargetScrollX = -(m_tInfo.fX - WINCX / 2);  // (m_tInfo.fX - WINCX / 2) -> 플레이어를 화면 중앙에 두려면 월드를 얼마나 옮겨야 하는지 
		float fTargetScrollY = -(m_tInfo.fY - WINCY / 2);
		CScrollMgr::Get_Instance()->Set_ScrollX_To(fTargetScrollX);
		CScrollMgr::Get_Instance()->Set_ScrollY_To(fTargetScrollY);

		CScrollMgr::Get_Instance()->Scroll_Lock();

	
	
		//이동할시 턴 종료 -> 적턴으로
		CTurnMgr::Get_Instance()->End_PlayerTurn();


	}
}
