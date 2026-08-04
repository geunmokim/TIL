#include <windows.h>

//DX11도 ImGui도 없음. 딱 "창 하나 + 메시지 루프"만 있는 제일 밑바닥 구조.

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//LRESULT ->정수형 타입 (이 메시지를 처리한 결과값을 담아서 돌려주는 용도
//WPARAM, LPARAM -> 메시지별로 다른 정보가 담기는 것
{
	switch (msg)
	{
	case WM_DESTROY:        //WM_DESTROY -> 이창이 닫혔다는걸 알려주는 메시지
		PostQuitMessage(0); //메시지큐에 WM_QUIT라는 특수 메시지를 하나 넣어라
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//윈도우가 WM_DESTROY 보냄 -> PostQuitMessage(0)으로 WM_QUIT큐에 넣어줘 요청
//->메인 루프의 GetMessage가 그걸 받고 false 리턴 -> while문 탈출 ->프로그램 완전 종료

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//hInstance -지금 실행중인 프로그램 자체를 가리키는 핸들 (창을 등록/생성 할때 어느 프로그램이 창의 주인인지를 알려주려고 계쏙 넘겨줌)
//hPrevInstance -옛날 프로그램이 실행중인지 체크하려고 쓰던건데 지금은 안쓰인다 그냥 형식상 남아있는 파라미터 항상 nullptr이 들어옴
//IpCmdLine - 프로그램 실행할떄 커맨드라인으로 넘긴 인자 -마찬가지로 지금은 안씀
//nCmdShow - 창을 처음에 어떻게 보여줄지 힌트값
{
	// 1. 창의 "종류"를 등록 (어떤 아이콘, 어떤 배경색, 메시지는 어디로 보낼지 등)
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;			// 이 창으로 오는 메시지는 WndProc 함수가 처리
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Day1WndClass";
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 기본 배경색 (아직 우리가 직접 그리는 게 아님)
	RegisterClassEx(&wc);

	// 2. 등록한 종류로 실제 창을 생성
	HWND hWnd = CreateWindow(
		wc.lpszClassName, L"Day1: 그냥 창 하나",
		WS_OVERLAPPEDWINDOW,
		100, 100, 800, 600,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 3. 메시지 루프: 창이 살아있는 동안 계속 반복하면서
	//    Windows가 보내는 메시지(마우스클릭, 창닫기 등)를 처리
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}