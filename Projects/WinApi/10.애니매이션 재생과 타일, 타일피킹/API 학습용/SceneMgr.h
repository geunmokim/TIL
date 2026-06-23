#pragma once
class CScene;
class CSceneMgr
{
public:
	enum SCENEID{LOGO, MENU, STAGE, EDIT, END};//열거체의 경우 클래스 내부에서 만들어서 사용가능하다
	                                               //클래스도 하나의 지역변수이기 때문

private:
	CSceneMgr();
	~CSceneMgr();
public:
	void Scene_Change(SCENEID _eID);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;

	}

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);

	}

private:
	static CSceneMgr* m_pInstance;
	CScene* m_pScene;

	SCENEID m_ePreScene;
	SCENEID m_eCurScene;

};

