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
	void Render();
	void Release();

private:
	HDC m_hDC;
	CObj* m_pPlayer;
	list<CObj*> m_listBullet;


};

