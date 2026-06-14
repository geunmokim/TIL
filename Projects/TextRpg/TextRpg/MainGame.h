#pragma once
//#include "Enum.h"         //예전에 배운 방식은 stdafx.h에 모든 헤더파일을 집어넣고 stdafx.h를 cpp에 넣는 방식이지만 현대는 필요한 부분만 이렇게 기재하는 식으로 바뀌었다
//class CPlayer;
//class CMonster;
class CObj;
class CShop;
class CField;
class CInventory;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	bool Initialize();
	void Progress();
	void Release();

private:
	void Select_Player();
private:
	void Save_Data();
	void Load_Data();
private:
	CObj*       m_pPlayer;
	CField*     m_pField;
	CShop*      m_pShop;
	CInventory* m_pInven;



};

