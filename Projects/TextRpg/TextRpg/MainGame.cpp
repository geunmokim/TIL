#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "Field.h"
#include "Shop.h"
#include "Inventory.h"
#include "Item.h"

CMainGame::CMainGame()
	:m_pPlayer(nullptr), m_pField(nullptr), m_pShop(nullptr),m_pInven(nullptr)
{   
	/*m_pPlayer = nullptr;
	m_pMonster = nullptr;*/
}

CMainGame::~CMainGame()
{
	Release();
}

bool CMainGame::Initialize()
{
	Select_Player();

	if (!m_pPlayer)
		return false;

	if (!m_pField)
	{
		m_pField = new CField;
		m_pField->Initialize();
	}
	if (!m_pShop)
	{
		m_pShop = new CShop;
		m_pShop->Initialize();
	}
	if (!m_pInven)
	{
		m_pInven = new CInventory;
		m_pInven->Initialize();
	}
	m_pField->Set_Player(m_pPlayer);
	m_pShop->Set_Player(m_pPlayer);
	m_pInven->Set_Player(m_pPlayer);

	m_pShop->Set_Inventory(m_pInven);


	return true;

}
void CMainGame::Progress()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << " 1.던전 2.인벤 3.상점 4.저장 5.종료" << endl;
		cout << "================================================" << endl;
		cout << "입력: ";
		cin >> iSelect;
		switch (iSelect)
		{
		case 1:
			m_pField->Progress();
			break;
		case 2:
			m_pInven->Progress();
			break;
		case 3:
			m_pShop->Progress();
			break;
		case 4:
			Save_Data();
			continue;
		case 5:
			return;
		}
	}
	/*int iSelect = 0;                                              //이 부분은 전부 필드클래스가 관리함
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << "1.초급 2. 중급 3. 고급 4.저장 5.종료" << endl;
		cout << "======================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			m_pMonster = new CMonster("슬라임", 5, 30, 1);
			m_pMonster->Initialize();
			break;
		case 2:
			m_pMonster = new CMonster("오우거", 30, 60, 2);
			m_pMonster->Initialize();
			break;
		case 3:
			m_pMonster = new CMonster("골렘", 60, 90, 3);
			m_pMonster->Initialize();
			break;
		case 4:
			Save_Data();
			continue;
		case 5:
			return;
		}
		switch (Combat())
		{
		case COMBAT_WIN:
			m_pPlayer->Combat_Win(m_pMonster->Get_Exp(), m_pMonster->Get_Gold());
			break;
		case COMBAT_LOSE:
			m_pPlayer->Combat_Lose();
			break;
		case COMBAT_RUN:
			m_pPlayer->Combat_Run();
			break;

		}
		SAFE_DELETE(m_pMonster);
	}*/

}
void CMainGame::Release()
{
	SAFE_DELETE(m_pField);
	SAFE_DELETE(m_pShop);
	SAFE_DELETE(m_pInven);

	SAFE_DELETE(m_pPlayer);
	//동적 할당 해제하는거 순서 중요함  
	//순서를 바꿧을떄 CInven::~CInven()이나 Release()에서 m_pPlayer->Get_Hp();같은 코드를 호출한다면 이미 삭제된 객체를 접근하게 되므로 프로그램이 터질 수 있음

}


void CMainGame::Select_Player()
{
	int iSelect = 0;

	while (true)
	{
		system("cls");
		cout << "1.전사 2.마법사 3.도적 4.불러오기 5.종료 ." << endl;
		cout << "===================================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			m_pPlayer = new CPlayer(INFO("전사", 10, 200));
			m_pPlayer->Initialize();
			return;
		case 2:
			m_pPlayer = new CPlayer(INFO("마법사", 30, 100));
			m_pPlayer->Initialize();
			return;
		case 3:
			m_pPlayer = new CPlayer(INFO("도적", 20, 150));
			m_pPlayer->Initialize();
			return;
		case 4:
			Load_Data();
			return;
		case 5:
			return;
		default:
			continue;
		}
	}
}

//COMBAT CMainGame::Combat()    //Main이 아닌 Field에서 관리함 
//{
//	int iSelect = 0;
//	while (true)
//	{
//		system("cls");
//		m_pPlayer->Render();
//		m_pMonster->Render();
//		cout << "1.전투 2.도망" << endl;
//		cout << "=========================================" << endl;
//		cout << "입력: ";
//		cin >> iSelect;
//
//		switch (iSelect)
//		{
//		case 1:
//			m_pPlayer->Set_Hp(m_pMonster->Get_Att());
//			m_pMonster->Set_Hp(m_pPlayer->Get_Att());
//			break;
//		case 2:
//			return COMBAT_RUN;
//		}
//		if (0 >= m_pPlayer->Get_Hp())
//		{
//			cout << "플레이어 사망!" << endl;
//			system("pause");
//			return COMBAT_LOSE;
//		}
//		if (0 >= m_pMonster->Get_Hp())
//		{
//			cout << "전투 승리!" << endl;
//			system("pause");
//			return COMBAT_WIN;
//		}
//	}
//}
void CMainGame::Save_Data()
{
	FILE* fp = nullptr;

	errno_t err = fopen_s(&fp, "../Data/Save.dat", "wb");

	if (0 == err)
	{
		fwrite(&static_cast<CPlayer*>(m_pPlayer)->Save_Data(), sizeof(INFO), 1, fp);
		
		//아래 코드는 장비,즉 인벤의 상태까지 저장
		for (size_t i = 0; i < m_pInven->Get_InvenSize(); ++i)
		{
			fwrite(&m_pInven->Get_ItemInfo(i), sizeof(INFO), 1, fp);
			fwrite(&m_pInven->Get_ItemState(i), sizeof(STATE), 1, fp);
			fwrite(&m_pInven->Get_ItemType(i), sizeof(TYPE), 1, fp);
		}

		cout << "저장 성공!" << endl;
		fclose(fp);
	}
	else
		cout << "저장 실패!" << endl;

	system("pause");
}

void CMainGame::Load_Data()
{
	FILE* fp = nullptr;

	errno_t err = fopen_s(&fp, "../Data/Save.dat", "rb");

	if (0 == err)
	{
		INFO tTemp = {};
		STATE eState = STATE_END;
		TYPE  eType = TYPE_END;

		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();


		fread(&tTemp, sizeof(INFO), 1, fp);

		static_cast<CPlayer*>(m_pPlayer)->Load_Data(tTemp);
		
		m_pInven = new CInventory;
		m_pInven->Initialize();

		while (true)
		{
			fread(&tTemp, sizeof(INFO), 1, fp);
			fread(&eState, sizeof(STATE), 1, fp);
			int iCnt = fread(&eType, sizeof(TYPE), 1, fp);

			if (1 > iCnt)
				break;

			CObj* pItem = new CItem(tTemp);
			pItem->Initialize();

			static_cast<CItem*>(pItem)->Set_State(eState);
			static_cast<CItem*>(pItem)->Set_Type(eType);

			m_pInven->Load_Item(pItem);
			if (STATE_EQUIP == eState)
				static_cast<CPlayer*>(m_pPlayer)->Load_Item(pItem, eType);
		}
		cout << "불러오기 성공!" << endl;
		fclose(fp);
	}
	else
		cout << "불러오기 실패!" << endl;

	system("pause");
}