#pragma once

enum TURN_STATE{PLAYER_TURN, ENEMY_TURN};//필요시 더 추가할것 
class CTurnMgr
{
private:
	CTurnMgr();
	~CTurnMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	//현재 누구의 턴인짖 다른 클래스가 확인할때 사용
	//플레이어의 턴이 아니면 NewPlayer::Key_Check()가 입력을 무시하게 만들 수 있다
	const TURN_STATE& Get_TurnState() const { return m_eTurnState; }


	void End_PlayerTurn();
	void Process_EnemyTurn();

public:
	static CTurnMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTurnMgr;
		return m_pInstance;


	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);

	}

private:
	static CTurnMgr* m_pInstance;
	TURN_STATE m_eTurnState;

};

