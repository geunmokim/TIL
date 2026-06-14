#include "stdafx.h"
#include "Obj.h"
#include "Field.h"
#include "Player.h"
#include "Monster.h"


CField::CField()
	:m_pPlayer(nullptr), m_pMonster(nullptr)
{

}

CField::~CField()
{
	Release();

}

void CField::Initialize()
{

}

void CField::Progress()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << "1.초급 2.중급 3.고급 4.나가기" << endl;
		cout << "======================================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			m_pMonster = new CMonster(INFO("슬라임", 5, 30, 1));
			m_pMonster->Initialize();
			break;
		case 2:
			m_pMonster = new CMonster(INFO("오우거", 30, 60, 2));
			m_pMonster->Initialize();
			break;
		case 3:
			m_pMonster = new CMonster(INFO("골렘", 60, 90, 3));
			m_pMonster->Initialize();
			break;
		case 4:
			return;
		default:
			continue;

		}
		switch (Combat())
		{
		case COMBAT_WIN:
			static_cast<CPlayer*>(m_pPlayer)->Combat_Win(static_cast<CMonster*>(m_pMonster)->Get_Exp(), static_cast<CMonster*>(m_pMonster)->Get_Gold());
			//실무에서는 이렇게 연속적으로 static 캐스팅을 사용하지 않는다 이건 교육용에 가까운 코드
			break;
		case COMBAT_LOSE:
			static_cast<CPlayer*>(m_pPlayer)->Combat_Lose(); 
			break;

		}
		SAFE_DELETE(m_pMonster);
	}
}
void CField::Release()
{

}

void CField::Set_Player(CObj* _pPlayer)
{
	m_pPlayer = _pPlayer;
}

COMBAT CField::Combat()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		m_pMonster->Render();
		cout << " 1.전투 2.도망 " << endl;
		cout << "=========================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			m_pPlayer->Set_Hp(-m_pMonster->Get_Att());
			m_pMonster->Set_Hp(-m_pPlayer->Get_Att());
			break;
		case 2:
			return COMBAT_RUN;

		}
		if (0 >= m_pPlayer->Get_Hp())
		{
			cout << "플레이어 사망!" << endl;
			system("pause");
			return COMBAT_LOSE;
		}

		if (0 >= m_pMonster->Get_Hp())
		{
			cout << "전투 승리!" << endl;
			system("pause");
			return COMBAT_WIN;
		}
	}
}