#pragma once
class CObj;
class CField
{
public:
	CField();
	~CField();

public:
	void Initialize();
	void Progress();
	void Release();
public:
	void Set_Player(CObj* _pPlayer);

private:
	COMBAT Combat();
private:
	CObj* m_pPlayer; //여러 객체를 통합 관리하면 이런식으로 사용
	//CPlayer* m_pPlayer; -> 사실 field는 Player를 전방선언해서 사용하기 떄문에 이렇게 사용하는게 캐스팅도 사용하지 않고 코드도 간결하다  //역할이 고정이면 이런식으로 사용
	CObj* m_pMonster;


};


