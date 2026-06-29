#pragma once


class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void Initialize() = 0;    
	virtual int Update() = 0;         
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;
	
	//몬스터에게 자기 턴이 왔음을 인지시키게끔 호출하는 함수
	virtual void On_Turn() {}
public:
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }
	void Set_bDead() { m_bDead = true; }
	void Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }
	void Set_PosX(float _x) { m_tInfo.fX += _x; }
	void Set_PosY(float _y) { m_tInfo.fY += _y; }
	void Set_ImageKey(const TCHAR* _pImageKey) { m_pImageKey = _pImageKey; }
	void Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }

	void Set_GridPos(int _row, int _col)
	{
		m_tGridPos.row = _row;
		m_tGridPos.col = _col;

		m_tInfo.fX = (float)(_col * TILECX);
		m_tInfo.fY = (float)(_row * TILECY);
	}


public:
	const RECT& Get_Rect() const { return m_tRect; }
	const INFO& Get_Info() const { return m_tInfo; }
	const int& Get_DrawID() const { return m_iDrawID; }
	const RENDERID::ID& Get_RenderID() const { return m_eRenderID; }
	const GRIDPOS& Get_GridPos() const { return m_tGridPos; }
	const bool& Is_Dead() const { return m_bDead; }

protected:
	void Update_Rect();    
	virtual void Update_Frame();
	                      
protected:
	INFO m_tInfo;
	RECT m_tRect;
	CObj* m_pTarget;
	FRAME m_tFrame;
	RENDERID::ID	m_eRenderID;

	float m_fSpeed;
	float m_fAngle;
	float m_fDis;

	bool m_bDead;

	const TCHAR* m_pImageKey;

	int m_iDrawID;

	GRIDPOS		m_tGridPos;
};

