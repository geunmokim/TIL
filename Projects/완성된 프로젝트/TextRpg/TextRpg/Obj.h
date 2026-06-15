#pragma once
class CObj                           //플레이어,몬스터, 아이템같은 오브젝트들의 최상위 부모
{
public:
	CObj();
	CObj(const INFO& _rInfo);
	~CObj();

public:
	virtual void Initialize() = 0;                     //자식들이 사용할 수 있게 virtual 선언
	virtual void Render() = 0;
	virtual void Release() = 0;
public:
	inline int Get_Att() { return m_tInfo.iAtt; }
	int Get_Hp() { return m_tInfo.iHp; }
	int Get_Gold() { return m_tInfo.iGold; }
	const char* Get_Name() const { return m_tInfo.szName; }
public:
	void Set_Hp(int _iAtt) { m_tInfo.iHp += _iAtt; }
	void Set_Gold(int _iGold) { m_tInfo.iGold += _iGold; }



protected:
	INFO m_tInfo;
};

