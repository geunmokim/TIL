#include "framework.h"
#include "MainGame.h"
#include "Player.h"

CMainGame::CMainGame()
	: m_pPlayer(nullptr)
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	if (!m_pPlayer)
	{
		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();

	}
	static_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_listBullet);
	//CMainGame   : m_listBullet 원본 소유 (전체 관리)
	//CPlayer     : m_pBullet으로 주소만 받아서 총알 추가만 함
	/*
	 → CPlayer 생성
	 → Set_Bullet(&m_listBullet)으로 원본 주소 넘겨줌
	 CPlayer::Update()에서
   → W 누르면 m_pBullet->emplace_back(Create_Bullet())
   → CMainGame의 m_listBullet에 직접 추가되는 것

	*/

}

void CMainGame::Update()
{
	m_pPlayer->Update();
	for (auto& pBullet : m_listBullet)  // "m_listBullet에 있는 모든 총알을 하나씩 꺼내서"
		pBullet->Update();  // "각각 Update() 실행해라"
}

void CMainGame::Render()
{
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);                       //사각형을 화면 일정부분에 덮어 줌으로써 잔상이 남는 것을 없애준다 ---- 총알이 발사하고 없어지는 것처럼 보이는 것도 이것 때문
	Rectangle(m_hDC, 100, 100, 500, 500);
	m_pPlayer->Render(m_hDC);

	for (auto& pBullet : m_listBullet)
		pBullet->Render(m_hDC);
}
void CMainGame::Release()
{
	for_each(m_listBullet.begin(), m_listBullet.end(), Safe_Delete<CObj*>);
	m_listBullet.clear();

	SAFE_DELETE(m_pPlayer);

	ReleaseDC(g_hWnd, m_hDC);
}