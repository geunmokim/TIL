#pragma once
#include "Obj.h"
//던전의 그리드 크기 //TileMgr가 쓰는 TILEX, TILEY와 동일한 의미로 맞춰둔다


class CNewPlayer : public CObj
{ public:
	CNewPlayer();
	virtual ~CNewPlayer();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	void Add_Atk(int _iVal) { m_iAtk += _iVal; }
	void Add_Def(int _iVal) { m_iDef += _iVal; }
	void Add_Exp(int _iExp);
	int Get_Atk() { return m_iAtk; }
	int Get_Def() { return m_iDef; }
	int Get_Hp() { return m_iHp; }
	int Get_Exp() { return m_iExp; }
	int Get_Level() { return m_iLevel; }
	int Get_MaxHp() { return m_iMaxHp; }
private:
	// 이동하려는 칸(_row, _col)이 던전 범위 안인지 검사.
	// 지금은 단순 범위 체크만 하고, 벽 충돌은 TileMgr 연동 단계에서 추가한다.
	bool Can_Move(int _row, int _col);
	void Level_Up();


private:
	void Key_Check();

private:
	bool m_bKeyLock;
	
	int m_iAtk;
	
	int m_iDef;

	bool m_bFlipX;
	
	int m_iExp;
	int m_iLevel;
	int m_iMaxExp;

	int m_iMaxHp;

};

