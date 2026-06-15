#include "stdafx.h"
#include "Monster.h"

CMonster::CMonster()
{
	memset(&m_tInfo, 0, sizeof(m_tInfo));

}

CMonster::CMonster(const INFO& _rInfo)
	:CObj(_rInfo)   //(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel)
{

	//밑에것들은 구버전의 잔재 신버전에선 Struct.h에서 tagInfo() {}
	//tagInfo(const char* _pName, int _iAtt, int _iMaxHp)
	//	:iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(1)
	//	, iMaxExp(100), iExp(0), iGold(0)
	//{
	//	strcpy_s(szName, sizeof(szName), _pName);
	//}
	//tagInfo(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel)
	//	:iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(_iLevel)
	//	, iMaxExp(0), iExp(0), iGold(0)
	//{
	//	strcpy_s(szName, sizeof(szName), _pName);

	//}

	//tagInfo(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel, int _iGold)   //객체를 생성하는 경우가 여러가지라서 오버로딩함
	//	: iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(_iLevel)
	//	, iMaxExp(0), iExp(0), iGold(_iGold)
	//{
	//	strcpy_s(szName, sizeof(szName), _pName);
	//}
	// 로 타입별로 정리해뒀기때문에 가져와서 쓰기만하면 됨 
	//strcpy_s(m_tInfo.szName, /*sizeof(m_tInfo.szName)*/16, _pName);
	//m_tInfo.iAtt = _iAtt;
	//m_tInfo.iMaxHp = _iMaxHp;
	//m_tInfo.iHp = _iMaxHp;
	//m_tInfo.iLevel = _iLevel;
	//m_tInfo.iMaxExp = 0;   //몬스터의 정보이므로 exp와 gold 모두 의미 없음
	//m_tInfo.iExp = 0;
	//m_tInfo.iGold = 0;
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.iMaxExp = (rand() % 50) + ((m_tInfo.iLevel - 1) * 50);  //레벨 구간에 따라 기본 경험치 + 0~49 랜덤을 더해서 최대 경험치를 설정   //rand() % 50 -> 0~49  //(m_tInfo.iLevel - 1) * 50 -> 레벨 구간 보정값
	m_tInfo.iExp = m_tInfo.iMaxExp;
	m_tInfo.iGold = (rand() % 50) + ((m_tInfo.iLevel - 1) * 50);
}

void CMonster::Render()
{
	cout << "이름: " << m_tInfo.szName << endl;
	cout << "공격력: " << m_tInfo.iAtt << endl;
	cout << "체력: " << m_tInfo.iHp << " / " << m_tInfo.iMaxHp << endl;
	cout << "==============================================" << endl;

}

void CMonster::Release()
{

}

int CMonster::Get_Exp()
{
	return m_tInfo.iExp;
}

//int CMonster::Get_Att()
//{
//	return m_tInfo.iAtt;
//}
//int CMonster::Get_Hp()
//{
//	return m_tInfo.iHp;
//}
//int CMonster::Get_Gold()
//{
//	return m_tInfo.iGold;
//}
//int CMonster::Get_Exp()
//{
//	return m_tInfo.iExp;
//}
//void CMonster::Set_Hp(int _iAtt)
//{
//	m_tInfo.iHp -= _iAtt;            //플레이어의 공격력만큼 hp 차감
//}