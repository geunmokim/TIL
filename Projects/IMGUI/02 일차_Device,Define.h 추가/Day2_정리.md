# Day2 정리: DX11 디바이스 붙이기

Day1의 창에 **그래픽카드 연결**을 추가해서, 창을 지정한 색으로 채우는 단계예요.

## 늘어난 파일

| 파일 | 역할 |
|---|---|
| `Define.h` | 싱글톤 매크로 (`DECLARE_SINGLETON`/`IMPLEMENT_SINGLETON`) |
| `Device.h/cpp` | 그래픽카드 연결 + 화면 지우기/띄우기 전담 |

## Define.h — 싱글톤 매크로

예전에 `Get_Instance`/`Destroy_Instance`를 클래스마다 손으로 반복해서 짜던 걸, 매크로로 뽑아서 재사용하는 것.

- `DECLARE_SINGLETON(ClassName)` → 헤더(`.h`)에서 클래스 안에 호출 → 함수/변수 **선언만**
- `IMPLEMENT_SINGLETON(ClassName)` → cpp 파일에서 클래스 밖에 호출 → 실제 **몸통(정의)**

선언/정의를 나눠놓은 이유: 몸통이 헤더에 그대로 있으면, 그 헤더를 여러 `.cpp`가 include할 때 "같은 함수가 중복 정의됐다"는 링크 에러가 날 수 있음. cpp 하나에만 몸통을 두면 이 문제 자체가 없음.

```cpp
#define DECLARE_SINGLETON(ClassName)   \
public:                                \
	static ClassName* Get_Instance();  \
	...
```
매크로가 여러 줄일 땐 각 줄 끝에 `\`를 붙여서 "다음 줄까지 이어짐"을 알려줌. **마지막 줄엔 `\` 없어야 함.** `\` 뒤에 공백 있으면 안 됨.

## Device.h/cpp — DX11 연결 (자세히 몰라도 되는 파트)

```cpp
HRESULT CDevice::Init_Device(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;        // 더블 버퍼링
	SwapChainDesc.OutputWindow = hWnd;    // Day1에서 만든 그 창
	...
	D3D11CreateDeviceAndSwapChain(..., &m_pSwapChain, &m_pDevice, ..., &m_pContext);
}
```

핵심 객체 4개, 표로 정리:

| 객체 | 역할 |
|---|---|
| `m_pDevice` | 그래픽 자원을 **만드는** 공장 |
| `m_pContext` | 실제로 **그리라고 명령** 내림 |
| `m_pSwapChain` | 화면 두 장(보이는 것 / 미리 그리는 것) 관리 |
| `m_pRenderTargetView` | "여기가 그릴 대상이다" 표시 |

DX9에선 `IDirect3DDevice9` 하나로 다 하던 걸, DX11부터 **Device(만들기)/Context(명령내리기)로 역할이 나뉜 것**만 기억하면 됨. 세부 파라미터 값들은 몰라도 되고, 필요할 때 공식 예제 그대로 복붙해서 씀 — 현업에서도 이 파트는 그렇게 씀.

```cpp
void CDevice::Render_Begin(...) { OMSetRenderTargets(...); ClearRenderTargetView(...); }
void CDevice::Render_End(...)   { m_pSwapChain->Present(1, 0); }
```
매 프레임: 그릴 대상 지정 + 화면 지우기(`Render_Begin`) → ... → 완성된 화면 띄우기(`Render_End`).

## Window.cpp — Day1과 다른 핵심 변화

**메시지 루프를 `GetMessage`(블로킹) → `PeekMessage`(논블로킹)로 교체.**

```cpp
while (msg.message != WM_QUIT)
{
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		continue;
	}

	// 처리할 메시지 없을 때만 실행 = 매 프레임 반복되는 부분
	CDevice::Get_Instance()->Render_Begin(ClearColor);
	CDevice::Get_Instance()->Render_End();
}
```
`GetMessage`는 메시지 올 때까지 프로그램을 멈춰서 화면을 다시 그릴 틈이 없음. `PeekMessage`는 메시지 없으면 그냥 넘어가서, 그 순간마다 렌더링 코드가 반복 실행됨 — 이게 게임/실시간 앱의 "메인 루프" 구조.

## 확인한 것
창이 회색 배경 대신 **어두운 남색으로 채워짐**을 확인. 아직 ImGui 없음, 딱 "DX11로 지정한 색 채우기"까지.
