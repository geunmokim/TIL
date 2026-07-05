#pragma once

//플레이어의 시야 
//시야 범위내는 visible, 아직 안가본 시야바깥은 UNSEEN ,탐험한 곳은  EXPLORED
// 몬스터의 시야는 On_Turn()에 따로 구현되어있기 떄문에 플레이어만 사용




class CSight
{
private:
	CSight();
	~CSight();


public:
	void Initialize();     //초기 시야 범위
	void Render(HDC _DC);  //시야 범위 랜더링
	void Release();       //소멸자 호출 시 


public:

	void Update_Sight();

public:
	static CSight* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSight;
		return m_pInstance;


	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}





private:
	static CSight* m_pInstance;
	vector<VISIBILITY> m_vecVisibility;  //타일의 상태값(UNSEEN,VISIBLE,EXPLORED)를 저장할 벡터 
	int m_iSize;
	

};

