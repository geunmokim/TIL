#pragma once
class CObj;
class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Render(HDC _DC);
	void Release();
public:
	void Picking_Tile(int _iDrawID);

public:
	void Save_Tile();
	void Load_Tile();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;

	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTileMgr* m_pInstance;
	vector<CObj*> m_vecTile;
	const size_t m_iSize;

};

