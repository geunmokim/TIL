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
	CObj* Get_Player() { return m_listObj[OBJID::PLAYER].front(); }
	CObj* Get_Target(OBJID::ID _eID, CObj* _pObj);
public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}

	/*
					       팩토리 Set_Target       ObjMgr Get_Target
    타겟 갱신                   고정                 매 프레임 갱신
    댕글링 포인터 위험          있음                    없음
    용도                 타겟이 바뀔 필요 없을 때    추적형 몬스터 등

     */

	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}
private:
	list<CObj*> m_listObj[OBJID::END];
	static CObjMgr* m_pInstance;

};

