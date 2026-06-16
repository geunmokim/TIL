#pragma once
class CObj;
class CMainGame
{

public:
	CMainGame();
	~CMainGame();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;
	CObj* m_pPlayer;
	CObj* m_pMonster;

	list<CObj*> m_listObj[OBJID::END] ; 

	int m_iFPS;
	DWORD m_dwTime;


	list<CObj*> m_listBullet;
};

