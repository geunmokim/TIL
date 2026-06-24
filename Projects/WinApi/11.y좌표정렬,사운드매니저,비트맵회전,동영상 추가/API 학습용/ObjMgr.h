#pragma once
class CObj;
class CObjMgr 
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj) { m_listObj[_eID].emplace_back(_pObj); }
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Delete_ID(OBJID::ID _eID);

public:
	CObj* Get_Player() { return m_listObj[OBJID::PLAYER].front(); }
	CObj* Get_Target(OBJID::ID _eID, CObj* _pObj);
public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}


	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	list<CObj*> m_listObj[OBJID::END];
	list<CObj*> m_listRender[RENDERID::END];
	static CObjMgr* m_pInstance;

};

