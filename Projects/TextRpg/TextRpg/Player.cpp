#include "stdafx.h"
#include "Player.h"
#include "Item.h"

CPlayer::CPlayer()
{
	memset(&m_tInfo, 0, sizeof(m_tInfo));    //구조체의 값을 0으로 초기화 하겠다
}

CPlayer::CPlayer(const INFO& _rInfo)    //(const char* _pName, int _iAtt, int _iMaxHp)          //Render()에서 정보를 받아와서 그것을 구조체에 입력해 그값을 사용한다
	:CObj(_rInfo)      //CObj::CObj(const INFO& _rInfo) : m_tInfo(_rInfo)
{
	//strcpy_s(m_tInfo.szName, sizeof(m_tInfo.szName), _pName);
	//m_tInfo.iAtt = _iAtt;
	//m_tInfo.iMaxHp = _iMaxHp;
	//m_tInfo.iHp = _iMaxHp;
	//m_tInfo.iLevel = 1;
	//m_tInfo.iMaxExp = 100;
	//m_tInfo.iExp = 0;
	//m_tInfo.iGold = 0;

}

CPlayer::~CPlayer()
{
	Release();

}
void CPlayer::Initialize()
{
	memset(m_pItem, 0, sizeof(m_pItem));

#ifdef _DEBUG
	m_tInfo.iGold = 100000;
#endif //_DEBUG
}

void CPlayer::Render()
{
	cout << "이름: " << m_tInfo.szName << endl;
	cout << "공격력: " << m_tInfo.iAtt << endl;
	cout << "체력: " << m_tInfo.iHp << " / " << m_tInfo.iMaxHp << endl;
	cout << "레벨: " << m_tInfo.iLevel << " ( " << m_tInfo.iExp << " / " << m_tInfo.iMaxExp << " ) " << endl;
	cout << "골드: " << m_tInfo.iGold << endl;
	cout << "============================================" << endl;
	Item_Info();

}

void CPlayer::Release()  
{

}
//int CPlayer::Get_Att()
//{
//	return m_tInfo.iAtt;
//}
//int CPlayer::Get_Hp()
//{
//	return m_tInfo.iHp;
//}
//void CPlayer::Set_Hp(int _iAtt)
//{
//	m_tInfo.iHp -= _iAtt;                       //몬스터의 공격력만큼 플레이어의 hp 차감
//}

void CPlayer::Combat_Win(int _iExp, int _iGold)    //전투 승리시 경험치와 골드에 관한 설정
{
	m_tInfo.iExp += _iExp;
	m_tInfo.iGold += _iGold;

	if (m_tInfo.iExp >= m_tInfo.iMaxExp)    //최대 경험치보다 경험치가 더 많게 된다면 
	{
		++m_tInfo.iLevel;
		m_tInfo.iExp -= m_tInfo.iMaxExp;
		m_tInfo.iMaxExp += 100;              //경험치 필요량 증가
		m_tInfo.iAtt += 10;                 
		m_tInfo.iMaxHp += 50;
		m_tInfo.iHp = m_tInfo.iMaxHp;        //증가한 최대 hp만큼 hp를 회복시킴
	}
}

void CPlayer::Combat_Lose()
{
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iGold >>= 1; //iGold= iGold /2와 같은 맥락 즉, 전투에서 진다면 골드와 경험치를 반토막 내겠다
	m_tInfo.iExp >>= 1;

}

void CPlayer::Combat_Run()
{
	m_tInfo.iGold = 0;
	m_tInfo.iExp = 0;
}

const INFO& CPlayer::Save_Data() const
{
	return m_tInfo;
}

void CPlayer::Load_Data(INFO& _rInfo)
{
	memcpy(&m_tInfo, &_rInfo, sizeof(INFO));  //_rInfo의 값을 복사해서 m_tInfo에 덮어쓴다
}

void CPlayer::Equip_Item(CObj* _pItem)
{
	TYPE eType = static_cast<CItem*>(_pItem)->Get_Type();

	if (m_pItem[eType])  //현재 장비가 장착되어 있는데 다른 장비로 교체할 경우
	{
		Item_Ability(-m_pItem[eType]->Get_Att(), -m_pItem[eType]->Get_Hp());     //여기서 -m_p_Item을 해주는 이유는 장비를 교체할떄 기존장착한 장비의 스탯을 제거하기 위해서
		//-를 붙이지 않으면 장비를 교체할때 기존 장비의 스탯도 +되어 원래값보다 높게 나오게 된다
		static_cast<CItem*>(m_pItem[eType])->Set_State(STATE_UNEQUIP);
		m_pItem[eType] = nullptr;
	}
	Item_Ability(_pItem->Get_Att(), _pItem->Get_Hp());  //장비가 장착되어 있지 않은 경우
	static_cast<CItem*>(_pItem)->Set_State(STATE_EQUIP);
	m_pItem[eType] = _pItem;

}

void CPlayer::UnEquip_Item(int _iIdx)
{
	if (m_pItem[_iIdx])
	{
		Item_Ability(-m_pItem[_iIdx]->Get_Att(), -m_pItem[_iIdx]->Get_Hp());   //위에 equip일떄 장비 장착했을떄 경우와 마찬가지로 장비를 해제하므로 -해서 장비의 스탯을 제거한다
		static_cast<CItem*>(m_pItem[_iIdx])->Set_State(STATE_UNEQUIP);
		m_pItem[_iIdx] = nullptr;
	}
}

void CPlayer::Item_Ability(int _iAtt, int _iMaxHp)
{
	m_tInfo.iAtt += _iAtt;
	m_tInfo.iMaxHp += _iMaxHp;

}


void CPlayer::Item_Info()
{
	for (int i = 0; i < TYPE_END; ++i)
	{
		if (m_pItem[i])
		{
			cout << i + 1 << ")" << m_pItem[i]->Get_Name() << endl;
			cout << "==============================================" << endl;
		}
	}
}