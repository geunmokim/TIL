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
	void Draw_MonsterWindow(void);
	void Draw_GridWindow(void);


private:
	//위젯이랑 실시간으로 연결된 변수들
	bool m_bCheck;
	float m_fValue;

	//day4 그리드 창에서 쓸 상태값
	int m_iSelectedSlot;

	//day5 흩어진 변수(m_bCheck, m_fValue) 대신 구조체 하나로 관리
	MonsterData m_Monster;


};

