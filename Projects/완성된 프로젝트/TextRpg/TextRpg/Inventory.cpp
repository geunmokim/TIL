#include "stdafx.h"
#include "Player.h"
#include "Shop.h"
#include "Inventory.h"
#include "Item.h"

CInventory::CInventory()
	:m_pPlayer(nullptr),m_iSize(5)
{

}

CInventory::~CInventory()
{
	Release();
}

void CInventory::Initialize()
{
	m_vecInven.reserve(m_iSize);
}

void CInventory::Progress()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << " ===========================================" << endl;
		cout << "1.장착 2.해제 3.나가기" << endl;
		cout << "================================" << endl;
		cout << "입력: ";
		cin >> iSelect;

		switch (iSelect)
		{
		case 1:
			Equip_Item();     //Equipment();
			break;
		case 2:
			UnEquip_Item();    //Take_Off();
			break;
		case 3:
			return;
		default:
			continue;


		}

	}
}
void CInventory::Render()
{
	for (size_t i = 0; i < m_vecInven.size(); ++i)
	{
		cout << i + 1 << ")" << endl;
		m_vecInven[i]->Render();


	}
}


void CInventory::Equip_Item()
{

	int iSelect = 0;
	while(true)
	{
		system("cls");
		m_pPlayer->Render();
		Render();
		cout << "0) 나가기" << endl;
		cout << "====================================================" << endl;
		cout << " 입력: ";
		cin >> iSelect;
		--iSelect;                 //m_vecInven은 배열이라 인덱스 0부터 시작하기 떄문에 

		if (0 > iSelect)
			return;

		if (m_vecInven.size() <= (size_t)iSelect)
			continue;

		static_cast<CPlayer*>(m_pPlayer)->Equip_Item(m_vecInven[iSelect]);

	}
}

void CInventory::UnEquip_Item()
{
	int iSelect = 0;
	while (true)
	{
		system("cls");
		m_pPlayer->Render();
		cout << "0)나가기" << endl;
		cout << "=====================================" << endl;
		cout << "입력: ";
		cin >> iSelect;
		--iSelect;

		if (0 > iSelect)
			return;
		if (TYPE_END < +iSelect)
			continue;

		static_cast<CPlayer*>(m_pPlayer)->UnEquip_Item(iSelect);

	}
}

//void CInventory::Add_Item(CObj* _pInven)
//{
//	vecInven.push_back(new CItem(static_cast<CItem*>(_pInven)->Get_Item()));
//	
//}

bool CInventory::Sell_Item(int _iIdx, int* _pGold)
{
	if (0 > _iIdx || m_vecInven.size() <= (size_t)_iIdx)
		return false;

	if (STATE_EQUIP == static_cast<CItem*>(m_vecInven[_iIdx])->Get_State())
		return false;

	vector<CObj*>::iterator iter = m_vecInven.begin();
	iter += _iIdx;

	*_pGold = (*iter)->Get_Gold() >> 1;   //판매가는 아이템 가격의 반값

	SAFE_DELETE(*iter);
	m_vecInven.erase(iter);
	return true;


}
bool CInventory::Buy_Item(CObj* _pItem)         //shop에서 아이템을 사면 인벤토리에 push_back해서 넣어둠
{
	if (m_vecInven.size() >= m_iSize)
		return false;

	CItem* pItem = static_cast<CItem*>(_pItem);
	CObj* newItem = new CItem(*pItem);
	m_vecInven.push_back(newItem);

	return true;

}

const INFO& CInventory::Get_ItemInfo(int _iIdx) const
{
	return static_cast<CItem*>(m_vecInven[_iIdx])->Get_Info();
}
const STATE& CInventory::Get_ItemState(int _iIdx) const
{
	return static_cast<CItem*>(m_vecInven[_iIdx])->Get_State();

}
const TYPE& CInventory::Get_ItemType(int _iIdx) const
{
	return static_cast<CItem*>(m_vecInven[_iIdx])->Get_Type();
}

//void CInventory::Set_Player(CObj* _pPlayer)
//{
//	m_pPlayer = _pPlayer;
//}
//

void CInventory::Release()
{
	for_each(m_vecInven.begin(), m_vecInven.end(), Safe_Delete<CObj*>); //단순히 컨테이너를 순회하면서 조건자를 수행한다
	                                                                    //컨테이너 원소로 동적할당한 주소를 저장할 경우 해제하는 용도로 사용한다 그래서 release()부분에 사용
	m_vecInven.clear();

}