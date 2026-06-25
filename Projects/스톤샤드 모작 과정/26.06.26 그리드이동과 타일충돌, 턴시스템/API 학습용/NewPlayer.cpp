#include "framework.h"
#include "NewPlayer.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "TurnMgr.h"
CNewPlayer::CNewPlayer()
{

}

CNewPlayer::~CNewPlayer()
{
	Release();
}

void CNewPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../image/Player/s_player_0.bmp"), _T("PlayerWalk"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;
	
	Set_GridPos(5, 5); // 시작 칸 위치. Set_GridPos가 m_tInfo.fX/fY까지 같이 맞춰준다

	m_pImageKey = _T("PlayerWalk");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;


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

	GdiTransparentBlt(_DC,
		m_tRect.left, m_tRect.top,
		m_tInfo.iCX, m_tInfo.iCY,
		hPlayerDC,
		0, 0,
		m_tInfo.iCX, m_tInfo.iCY,
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
	if (CTileMgr::Get_Instance()->Is_Wall(_row, _col))
		return false;

	//범위 안이라도 그 칸이 벽이라면 이동 불가(DrawaID 기준으로 판단)
	if (CTileMgr::Get_Instance()->Is_Wall(_row, _col))
		return false;

	return true;
}

void CNewPlayer::Key_Check()
{
	//플레이어 턴이 아니라면 키를 입력해도 무효화
	if (PLAYER_TURN != CTurnMgr::Get_Instance()->Get_TurnState())
		return;


	int row = m_tGridPos.row;
	int col = m_tGridPos.col;

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		col -= 1;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		col += 1;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		row -= 1;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		row += 1;
	}
	else
		return; //입력이 없을 시 아무것도 안함

	if (Can_Move(row, col))
		Set_GridPos(row, col);

	//이동할시 턴 종료 -> 적턴으로
	
	CTurnMgr::Get_Instance()->End_PlayerTurn();



}
