#pragma once
class CObj;
class CInventory;
class CShop
{
public:
	CShop();
	~CShop();
public:
	void Initialize();
	void Progress();
	void Render(LEVEL _eLevel);
	void Release();
public:
	void Buy_Item(CObj* _pItem);
	void Sell_Item();

public:
	void Set_Player(CObj* _pPlayer);
	void Set_Inventory(CInventory* _pInven){ m_pInven = _pInven; }

private:
	CObj* m_pPlayer;                       //플레이어의 정보를 가져오기 위해서
	CObj* m_pItemArr[LEVEL_END][TYPE_END]; // 아이템 배열 
	CInventory* m_pInven;
};

