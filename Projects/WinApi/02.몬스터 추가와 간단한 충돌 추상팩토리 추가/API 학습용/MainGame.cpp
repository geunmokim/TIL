#include "framework.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"

CMainGame::CMainGame()
	: m_iFPS(0), m_dwTime(GetTickCount())
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	CObj* pObj = CAbFactory<CPlayer>::Create(); //호출부 - 반환값을 받을떄 포인터로 받음
	// 여기서 이미
    // 1. new CPlayer로 메모리 할당
    // 2. Initialize()로 위치, 크기, 속도 설정
    // 완성된 플레이어가 pObj에 있음
	// 근데 pObj는 그냥 지역변수
    // Initialize()가 끝나면 이 함수(CMainGame::Initialize)가 끝날때 사라져버림

	static_cast<CPlayer*>(pObj)->Set_Bullet(&m_listObj[OBJID::BULLET]);   //총알 리스트의 주소를 플레이어한테 알려줌 //m_listObj[OBJID::BULLET] → 총알들이 저장될 리스트

	m_listObj[OBJID::PLAYER].emplace_back(pObj);
	//pObj가 지역변수라서 함수가 끝날때 플레이어의 정보가 사라져 버리니 list에 보관함

	pObj = CAbFactory<CMonster>::Create();
	m_listObj[OBJID::MONSTER].emplace_back(pObj);
	//몬스터도 플레이어와 마찬가지로
	//pObj = CAbFactory<CMonster>::Create(); 여기서 몬스터를 완성했지만 pObj는 지역변수로 함수가 끝날떄 사랒짐
	//그래서 list로 몬스터의 정보를 보관함 //emplace_back을 사용하는 이유는 push_back보다 성능이 좋기 때문




	//아래 코드는 총알 삭제 추가 이전 코드
	//if (!m_pPlayer)
	//{
	//	m_pPlayer = new CPlayer;
	//	m_pPlayer->Initialize();

	//}
	//static_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_listBullet);
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
	//m_pPlayer->Update();
	//for (auto& pBullet : m_listBullet)  // "m_listBullet에 있는 모든 총알을 하나씩 꺼내서"
	//	pBullet->Update();  // "각각 Update() 실행해라"
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
}
void CMainGame::Late_Update()
{
	RECT rec = {};
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
			pObj->Late_Update();

	}
	for (auto& pObj : m_listObj[OBJID::BULLET])
	{
		for (auto& pColObj : m_listObj[OBJID::MONSTER])
		{
			if (IntersectRect(&rec, &pObj->Get_Rect(), &pColObj->Get_Rect()))
			{
				pObj->Set_bDead(true);
				pColObj->Set_bDead(true);
			}
		}
	}
	/*
	1. 총알 리스트 순회
    2. 각 총알마다 몬스터 리스트 전체와 충돌 체크
    3. IntersectRect로 두 Rect가 겹치면
     → 총알 m_bDead = true
     → 몬스터 m_bDead = true
    4. 다음 Update()에서 OBJ_DEAD 반환
     → 리스트에서 삭제

	 왜 Late_Update에서 하냐면
      Update()    → 이동, 입력 처리
      Late_Update → 이동 다 끝난 후 충돌 체크
	*/
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