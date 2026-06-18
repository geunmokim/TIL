#include "framework.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "CollisionMgr.h"
CMainGame::CMainGame()
	: m_pPlayer(nullptr),m_iFPS(0), m_dwTime(GetTickCount())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	if (m_listObj[OBJID::PLAYER].empty())
	{

		CObj* pObj = CAbFactory<CPlayer>::Create();
		static_cast<CPlayer*>(pObj)->Set_Bullet(&m_listObj[OBJID::BULLET]);
		static_cast<CPlayer*>(pObj)->Set_Shield(&m_listObj[OBJID::SHIELD]);

		m_listObj[OBJID::PLAYER].emplace_back(pObj);

	}

	CObj* pObj = CAbFactory<CMonster>::Create(400.f, 200.f); //이전코드에선 몬스터 initialize()에서 몬스터의 시작 위치를 정했는데 이젠 Create()할때마다 내가 원하는 위치에서 생성
	m_listObj[OBJID::MONSTER].emplace_back(pObj);

}

void CMainGame::Update()
{

	for (int i = 0; i < OBJID::END; ++i) //for문을 통해 플레이어, 총알, 몬스터의 업데이트를 한번에 돌림
	{
		auto iter = m_listObj[i].begin();   //player,bullet,monster가 모두 m_listObj[]에 들어가 있으므로 업데이트가 순차적으로 됨
		for (; iter != m_listObj[i].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[i].erase(iter);

			}
			else
				++iter;
		}
	}
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::MONSTER], m_listObj[OBJID::BULLET]);  //몬스터와 총알의 충돌 판정
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);  //플레이어와 몬스터의 충돌 판정
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::SHIELD], m_listObj[OBJID::MONSTER]);  //실드와 몬스터의 충돌 판정
}
void CMainGame::Late_Update()
{
	RECT rec = {};
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
			pObj->Late_Update();
	}

}

void CMainGame::Render()
{
	Rectangle(m_hDC, 0, 0, WINCX, WINCY);                       //사각형을 화면 일정부분에 덮어 줌으로써 잔상이 남는 것을 없애준다 ---- 총알이 발사하고 없어지는 것처럼 보이는 것도 이것 때문
	Rectangle(m_hDC, 100, 100, WINCX - 100, WINCY - 100);

	for (int i = 0; i < OBJID::END; ++i)    //직접 호출방식 대신 for문을 사용하는건 몬스터,총알 같이 여러개의 오브젝트가 등장할 경우 필수적이라
	{
		for (auto& pObj : m_listObj[i])
			pObj->Render(m_hDC);
	}

#ifdef _DEBUG
	TCHAR		szBuff[32] = L"";
	swprintf_s(szBuff, L"Bullet: %d", m_listObj[OBJID::BULLET].size());
	TextOut(m_hDC, 50, 50, szBuff, lstrlen(szBuff));
	/*
	현재 총알 리스트 개수를 화면 (50, 50) 위치에 출력
    → 디버깅용 (총알이 제대로 생성/소멸되는지 확인)
	
	*/
	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		TCHAR	szFPS[16] = L"";
		swprintf_s(szFPS, L"FPS: %d", m_iFPS);
		SetWindowText(g_hWnd, szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
	/*
	1초동안 Render()가 몇 번 호출됐는지 세서
    → 창 타이틀바에 "FPS: 60" 이런식으로 표시
	
	*/
#endif



	/*m_pPlayer->Render(m_hDC);        //직접 호출방식 -> 플레이어같이 1개만 필요한건 상관없지만 몬스터,bullet같이 여러개의 오브젝트를 다룰땐 사용하지 않는다

	for (auto& pBullet : m_listBullet)
		pBullet->Render(m_hDC);*/
}
void CMainGame::Release()
{

	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>); //리스트를 순회하면서 삭제해라
		m_listObj[i].clear();
	}
	/*for_each(m_listBullet.begin(), m_listBullet.end(), Safe_Delete<CObj*>);
	m_listBullet.clear();

	SAFE_DELETE(m_pPlayer);*/

	ReleaseDC(g_hWnd, m_hDC);
}