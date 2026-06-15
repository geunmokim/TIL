#include "stdafx.h"
#include "Shop.h"
#include "Player.h"
#include "Item.h"
#include "Inventory.h"

CShop::CShop()
	:m_pPlayer(nullptr), m_pInven(nullptr)
{
	memset(m_pItemArr, 0, sizeof(m_pItemArr)); //아이템 배열에 전부 0을 채워넣어라
}

CShop::~CShop()
{
	Release();
}

void CShop::Initialize()
{
	CObj* pItem = new CItem(INFO("초급 검", 10, 0, 1, 100));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_SWORD);
	m_pItemArr[LEVEL_1][TYPE_SWORD] = pItem;

	pItem = new CItem(INFO("초급 옷", 0, 50, 1, 100));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_ARMOR);
	m_pItemArr[LEVEL_1][TYPE_ARMOR] = pItem;

	pItem = new CItem(INFO("중급 검", 30, 0, 1, 500));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_SWORD);
	m_pItemArr[LEVEL_2][TYPE_SWORD] = pItem;

	pItem = new CItem(INFO("중급 옷", 0, 150, 1, 500));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_ARMOR);
	m_pItemArr[LEVEL_2][TYPE_ARMOR] = pItem;

	pItem = new CItem(INFO("고급 검", 60, 0, 1, 1000));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_SWORD);
	m_pItemArr[LEVEL_3][TYPE_SWORD] = pItem;

	pItem = new CItem(INFO("고급 옷", 0, 300, 1, 1000));
	pItem->Initialize();
	static_cast<CItem*>(pItem)->Set_Type(TYPE_ARMOR);
	m_pItemArr[LEVEL_3][TYPE_ARMOR] = pItem;
}
void CShop::Progress()
{
	int iSelect = 0;

	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << "1.초급 2.중급 3.고급 4.판매 5.나가기" << endl;
		cout << " ===========================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			Render(LEVEL_1);
			break;
		case 2:
			Render(LEVEL_2);
			break;
		case 3:
			Render(LEVEL_3);
			break;
		case 4:
			Sell_Item();
			break;
		case 5:
			return;
		default:
			continue;

		}
	}

}

void CShop::Render(LEVEL _eLevel)
{
	int iSelect = 0;

	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		m_pItemArr[_eLevel][TYPE_SWORD]->Render();
		m_pItemArr[_eLevel][TYPE_ARMOR]->Render();
		cout << "1.무기구매 2.방어구구매 3.나가기" << endl;
		cout << "=================================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			Buy_Item(m_pItemArr[_eLevel][TYPE_SWORD]);
			break;
		case 2:
			Buy_Item(m_pItemArr[_eLevel][TYPE_ARMOR]);
			break;
		case 3:
			return;
		default:
			continue;
		}
	}
}

void CShop::Release()
{
	for (int i = 0; i < LEVEL_END; ++i)
	{
		for (int j = 0; j < TYPE_END; ++j)
			SAFE_DELETE(m_pItemArr[i][j]);
	}
}

void CShop::Buy_Item(CObj* _pItem)
{
	if (m_pPlayer->Get_Gold() >= _pItem->Get_Gold() && m_pInven->Buy_Item(_pItem))
	{
		m_pPlayer->Set_Gold(-_pItem->Get_Gold());
		cout << "구매 성공!" << endl;
		/*m_pInven->Add_Item(_pItem);*/

	}
	else
		cout << "구매 실패!" << endl;

	system("pause");
}
void CShop::Sell_Item()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		m_pInven->Render();
		cout << "0)나가기" << endl;
		cout << "=========================================" << endl;
		cout << "입력: ";
		cin >> iSelect;
		--iSelect;

		if (0 > iSelect)
			return;

		int iGold = 0;
		if (m_pInven->Sell_Item(iSelect, &iGold))
		{
			m_pPlayer->Set_Gold(iGold);
			cout << "판매 성공!" << endl;
		}
		else
			cout << "판매 실패! " << endl;
		system("pause");
	}
}

void CShop::Set_Player(CObj* _pPlayer)
{
	m_pPlayer = _pPlayer;
}
//void CShop::Set_Inventory(CInventory* _pInventory)
//{
//	m_pInven = _pInventory;
//}