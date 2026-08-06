# Day1 정리: 창 하나 + 메시지 루프

DX11도 ImGui도 없이, 순수 WinAPI로 **제일 밑바닥 구조**만 확인한 단계예요.

## 코드 흐름

```cpp
int APIENTRY WinMain(HINSTANCE hInstance, ...)
{
	// 1. 창의 "종류"를 등록
	WNDCLASSEX wc = {};
	wc.lpfnWndProc = WndProc;      // 이 창으로 오는 메시지는 WndProc이 처리
	wc.hInstance = hInstance;
	RegisterClassEx(&wc);

	// 2. 등록한 종류로 실제 창 생성
	HWND hWnd = CreateWindow(...);
	ShowWindow(hWnd, nCmdShow);

	// 3. 메시지 루프
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
```

## 핵심 개념 3가지

**① `WinMain`** — Windows 프로그램의 시작점(진입점). 콘솔의 `main()`과 같은 역할.
- `hInstance`: 지금 실행 중인 이 프로그램 핸들 (창 만들 때 필수로 넘겨줌)
- `hPrevInstance`: 옛날에 쓰던 것, 지금은 안 씀, 항상 nullptr
- `lpCmdLine`: 커맨드라인 인자 문자열, 지금은 안 씀
- `nCmdShow`: 창을 처음에 어떻게 보여줄지 힌트값

**② `WndProc`** — Windows가 우리 창으로 메시지(마우스 클릭, 창 닫기 등) 보낼 때마다 **자동으로 호출**해주는 콜백 함수.
```cpp
case WM_DESTROY:            // "이 창이 닫혔다"는 메시지
    PostQuitMessage(0);     // 메시지 큐에 WM_QUIT을 넣어달라고 요청
    return 0;
```
`WM_DESTROY` → `PostQuitMessage(0)` → 메인 루프의 `GetMessage`가 `WM_QUIT`을 받고 false 리턴 → `while`문 탈출 → 프로그램 종료. 이 줄 없으면 창은 사라져도 프로세스는 백그라운드에 좀비로 남음.

`DefWindowProc`은 우리가 처리 안 한 나머지 메시지(최소화, 크기조절 등)를 Windows 기본 동작에 맡기는 것.

**③ 타입 이름 관례**
- `LRESULT`: 메시지 처리 결과를 담는 정수 타입. WinAPI 전용.
- `WPARAM`, `LPARAM`: 메시지별로 다른 정보가 담기는 "봉투". 내용물은 메시지 종류마다 다름 — 그때그때 찾아 쓰면 됨.
- `HRESULT` (Day2에서 등장): WinAPI가 아니라 **COM** 규약. 성공/실패 + 실패 원인까지 한 번에 담는 정수. `FAILED()`/`SUCCEEDED()` 매크로로 체크.

## 확인한 것
창 하나 뜨고, X 버튼으로 닫으면 깔끔하게 프로세스 종료되는 것까지 확인. 아직 우리가 직접 그린 건 없고 Windows 기본 배경색만 보임.
