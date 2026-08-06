#pragma once
#include "Define.h"
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
	//위젯이랑 실시간으로 연결된 변수들
	bool m_bCheck;
	float m_fValue;

};

