#pragma once
class CObj;
class CObjMgr 
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj) { m_listObj[_eID].emplace_back(_pObj); }
	void Add_Object(RENDERID::ID _eID, CObj* _pObj) { m_listRender[_eID].emplace_back(_pObj); }
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	void Delete_ID(OBJID::ID _eID);

public:
	CObj* Get_Player() { return m_listObj[OBJID::PLAYER].front(); }
	CObj* Get_Target(OBJID::ID _eID, CObj* _pObj);

	// TurnMgr가 몬스터 턴에 몬스터 전체를 순회하며 한 번씩 행동시키기 위해 추가
	const list<CObj*>& Get_MonsterList() { return m_listObj[OBJID::MONSTER]; }

	
	//플레이어와 몬스터가 같은 칸에서 겹치는걸 막기위해 사용
	//NewPlayer::Can_Move()에서는 몬스터와 같은 칸이 되는걸 막는 방지용도로
	//몬스터의 On_Turn()에서는 이동시 플레이어와 겹치면 이동 대신 공격을 하게끔 
	bool Is_Occupied(const GRIDPOS& _pos);
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

