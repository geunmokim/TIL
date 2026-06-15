#pragma once
class CObj;
class CInventory
{
public:
	CInventory();
	~CInventory();
public:
	virtual void Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();

public:
	bool Buy_Item(CObj* _pItem);
	bool Sell_Item(int _iIdx, int* _pGold);
public:
	const size_t Get_InvenSize() const { return m_vecInven.size(); }
	const INFO& Get_ItemInfo(int _iIdx)const;
	const STATE& Get_ItemState(int _iIdx)const;
	const TYPE& Get_ItemType(int _iIdx) const;

public:
	void Load_Item(CObj* _pItem) { m_vecInven.push_back(_pItem); }

//public:
//
//	void Sell_Item();
public:
	void Equip_Item();
	void UnEquip_Item();
public:
	void Set_Player(CObj* _pPlayer) { m_pPlayer = _pPlayer; }
private:
	vector<CObj*> m_vecInven;
	CObj* m_pPlayer;

	const size_t m_iSize;


};

