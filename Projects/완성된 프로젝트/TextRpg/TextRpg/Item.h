#pragma once
#include "Obj.h"
class CItem :public CObj
{
public:
	CItem();
	CItem(const INFO& _rInfo);
	CItem(const CItem& _rItem);
	virtual ~CItem();

public:
	virtual void Initialize() override;      
	virtual void Render() override;              //물려받은 것중 실사용은 Render만
	virtual void Release() override;
public:
	void Set_Type(TYPE _eType) { m_eType = _eType; }
	void Set_State(STATE _eState) { m_eState = _eState; }
public:
	const INFO& Get_Info() const { return m_tInfo; }
	const STATE& Get_State() const { return m_eState; }
	const TYPE& Get_Type() const { return m_eType; }
private:
	TYPE  m_eType;
	STATE m_eState;

//public:
//	INFO& Get_Item() { return m_tInfo; }        //다른곳에서(Inventory) iTem 정보에 관해 사용 
};

