#include "stdafx.h"
#include "Item.h"

CItem::CItem()
	:m_eType(TYPE_END),m_eState(STATE_END)               
	//m_eType(TYPE_END)
	//m_eState(STATE_END)
	//로 초기화한 이유는
	//쓰레기값 방지 (TYPE m_eType, STATE m_eState 라고만 하고 초기화를 안하면 쓰레기값이 들어가기 때문)
	//"아직 설정되지 않음"을 표현
	//잘못된 사용을 감지하기 쉬움
	//enum의 마지막 값(*_END)을 유효하지 않은 sentinel 값으로 활용하기 위함
{

}

CItem::CItem(const INFO& _rInfo)
	:CObj(_rInfo),m_eType(TYPE_END),m_eState(STATE_END)
{

}
CItem::CItem(const CItem& _rItem)
	: CObj(_rItem.m_tInfo)
{
	m_eType = _rItem.m_eType;
	m_eState = _rItem.m_eState;
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_eState = STATE_UNEQUIP;
}
void CItem::Render()
{
	if (STATE_EQUIP == m_eState)
		cout << "<<<<<<<<<<<<착용중>>>>>>>>>>>>>>>" << endl;
	cout << "이름: " << m_tInfo.szName << endl;
	cout << "공격력: +" << m_tInfo.iAtt << endl;
	cout << "체력: +" << m_tInfo.iMaxHp << endl;
	cout << "골드: " << m_tInfo.iGold << endl;
	cout << "=======================================================" << endl;
	
}
void CItem::Release()
{

}