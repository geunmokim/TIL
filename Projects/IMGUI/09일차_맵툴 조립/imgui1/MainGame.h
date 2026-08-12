#pragma once
#include "Define.h"
#include "Struct.h"

class CMainGame
{
	DECLARE_SINGLETON(CMainGame)

private:
	CMainGame();
	~CMainGame();

public:
	HRESULT Init_ImGui(HWND hWnd);
	void Update(void); //이번 프레임에 그릴 위젯을 정의
	void Render(void);  //실제로 그리기
	void Release(void);


private:
	//day4 창을 역할별로 나눠서 그리는 함수 3개
	void Draw_ToolbarWindow(void);
	void Draw_MonsterListWindow(void);
	void Draw_GridWindow(void);       //DAY9 이제 팔레트 역할 겸함
	void Draw_MapWindow(void);         //DAY9 추가 실제 맵 격자


private:
	void Save_Monsters(void);
	void Load_Monsters(void);

	void Save_Map(void);   //day9 추가
	void Load_Map(void);   //day9 추가


private:
	HWND		m_hWnd;
	int m_iSelectedSlot;       //팔레트에서 선택된 타일 종류 (= 현재 찍을 타일)
	
	//day6 몬스터 1마리(m_Monster) 대신 배열로 관리
	static const int MONSTER_COUNT = 3;

	MonsterData m_Monsters[MONSTER_COUNT];

	int m_iSelectedMonster; //리스트에서 몇 번째가 선택됐는지

	int m_iMonsterType; //day6 combo용 - 몬스터 종족 선택

	// [Day7] 로드한 텍스처를 들고 있을 변수. ID3D11ShaderResourceView*가 곧 ImGui::Image가 요구하는 타입
	ID3D11ShaderResourceView* m_pIconTexture;
	int							m_iIconWidth, m_iIconHeight;

	//day9추가 맵 데이터 - 각 칸에 어떤 타일 종류가 찍혀있는지
	int  m_MapData[MAP_HEIGHT][MAP_WIDTH];

};

