#pragma once
#include "Obj.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	CPlayer(const INFO& _rInfo);       //(const char* _pName, int _iAtt, int _iMaxHp); //char*로 한 이유는 다른데서 이름을 수정하기 위해서
	~CPlayer();

public:
	virtual void Initialize();
	virtual void Render();
	virtual void Release();
//public:
//	int Get_Att();
//	int Get_Hp();
//public:
//	void Set_Hp(int _iAtt);  //전투로 인한 hp변동
public:
	void Combat_Win(int _iExp, int _iGold);   //전투 승리시 경험치와 소지골드에 변동을 주기 위해
	void Combat_Lose();   
	void Combat_Run();

public:
	const INFO& Save_Data() const;
	void Load_Data(INFO& _rInfo);
	void Load_Item( CObj* _pItem, TYPE _eType){m_pItem[_eType] = _pItem; }
public:
	void Equip_Item(CObj* _pItem);
	void UnEquip_Item(int _iIdx);
	void Item_Ability(int _iAtt, int _iMaxHp);

private:
	void Item_Info();

private:
	CObj* m_pItem[TYPE_END];
	



};

