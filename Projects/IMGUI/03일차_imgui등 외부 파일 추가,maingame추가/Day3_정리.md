# Day3 정리: ImGui 처음 붙이기

## 1. Day2 대비 뭐가 늘어났나

| 파일 | 상태 | 역할 |
|---|---|---|
| `Define.h` | 그대로 | 싱글톤 매크로 (안 바뀜) |
| `Device.h` | 2줄 추가 | `Get_Device()`, `Get_Context()` — ImGui한테 그래픽 연결정보 넘겨주려고 |
| `Device.cpp` | 그대로 | DX11 초기화/화면 지우기·띄우기 (안 바뀜) |
| `MainGame.h/cpp` | **새 파일** | ImGui 켜고, 위젯 그리고, 끄는 걸 전담하는 클래스 |
| `Window.cpp` | 3곳 추가 | 입력 전달 / ImGui 초기화 호출 / 매 프레임 Update·Render 호출 |
| `imgui` 라이브러리 | **새로 추가** | GitHub에서 받은 것. 우리가 손댈 일 없음 |

## 2. 실행 흐름 (매 프레임 반복되는 부분)

```
CMainGame::Update()          ← 이번 프레임에 뭘 그릴지 "정의"만 함 (아직 화면엔 안 그림)
CDevice::Render_Begin()      ← 화면 지우기
CMainGame::Render()          ← Update에서 정의한 걸 실제로 그리기
CDevice::Render_End()        ← 화면에 띄우기(Present)
```

**핵심**: 화면에 뭐가 보일지는 전부 `MainGame.cpp`의 `Update()` 안에서 결정돼요. 나머지(Device, imgui 라이브러리, backends)는 그걸 실제로 화면에 띄워주는 배경 장치일 뿐, 앞으로 UI 내용을 바꿀 땐 계속 `Update()`만 건드리게 될 거예요.

## 3. "그냥 있다고만 알면 되는 것" vs "익혀야 하는 것"

**있다고만 알면 되는 것** (Device랑 동급, 한 번 세팅하면 안 건드림):
```cpp
ImGui::CreateContext();
ImGui_ImplWin32_Init(hWnd);
ImGui_ImplDX11_Init(...);
...
ImGui::Render();
ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
...
ImGui_ImplDX11_Shutdown();
ImGui_ImplWin32_Shutdown();
ImGui::DestroyContext();
```

**직접 익혀야 하는 것** (`Update()` 안, 앞으로 계속 손으로 쓸 부분):
```cpp
if (ImGui::Button(u8"버튼")) { ... }              // 눌린 순간 true 리턴
ImGui::Checkbox(u8"체크박스", &m_bCheck);           // 변수 주소 직접 연결
ImGui::SliderFloat(u8"슬라이더", &m_fValue, 0.0f, 100.0f);
//                   라벨            변수주소      최소     최대
```
대부분 위젯이 **"라벨 → 연결할 변수 주소 → (필요하면) 범위"** 패턴을 반복해요.

## 4. 이번에 겪은 문제 2가지와 원인

1. **한글이 `?`로 나옴** → ImGui 기본 폰트에 한글 글리프가 없어서. `Init_ImGui`에 `io.Fonts->AddFontFromFileTTF(..., GetGlyphRangesKorean())`으로 맑은고딕 로드해서 해결.
2. **`imgui_demo.cpp` 컴파일 에러** → 데모 창의 일본어 샘플 문자열 하나가 우리 프로젝트 설정이랑 안 맞아서. 그 줄만 빈 문자열로 교체해서 해결 (데모 기능엔 영향 없음).

## 5. 직접 확인해서 이해한 것 — "즉시모드" 체감

```cpp
if (m_bCheck)
    ImGui::Text(u8"체크됨! 값: %.1f", m_fValue);
```
체크박스 켰다 껐다 하면 텍스트가 나타났다 사라졌다 하는 걸 확인하셨죠. MFC였으면 `OnCheckboxClicked` 이벤트 핸들러 만들고 `SetVisible` 호출해야 했을 걸, 여기선 매 프레임 다시 그려지는 구조 덕분에 `if`문 하나로 끝남 — 이게 Day1에서 말한 "즉시모드"가 실제로 동작하는 모습이에요.

## Day4 예고
지금은 창 하나에 위젯 여러 개가 다 들어있는 상태예요. Day4에서는 이걸 **여러 개의 창으로 나눠서 배치**하는 걸 배워요 (`SetNextWindowPos`, `Columns`, `BeginChild` 등).
