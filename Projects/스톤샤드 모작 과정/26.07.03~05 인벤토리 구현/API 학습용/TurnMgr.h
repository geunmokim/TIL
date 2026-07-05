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

	// 플레이어가 죽어서 더 이상 턴이 진행되지 않는 상태인지 확인.
	const bool& Is_GameOver() const { return m_bGameOver; }

	//플레이어의 턴이 끝나면 STATE 전환하기 위해
	void End_PlayerTurn();


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
	//어느 몬스터가 턴을 실행할건지 //죽은 상태등등이면 턴을 못가지게 관리를 위해
	void Process_EnemyTurn();
private:
	static CTurnMgr* m_pInstance;
	TURN_STATE m_eTurnState;
	bool m_bGameOver;
};

