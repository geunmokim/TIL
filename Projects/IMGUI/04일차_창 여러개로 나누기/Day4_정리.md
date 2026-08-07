# Day4 정리: 창 여러 개로 나누기

## 1. Day3 대비 뭐가 늘어났나

| 파일 | 상태 | 역할 |
|---|---|---|
| `Define.h`, `Device.h/cpp` | 그대로 | 안 바뀜 |
| `MainGame.h/cpp` | 함수 3개로 분리 | `Draw_ToolbarWindow` / `Draw_SettingsWindow` / `Draw_GridWindow` |
| `Window.cpp` | 창 제목만 변경 | 로직은 그대로 |

Day3까지는 창 하나에 위젯을 다 몰아넣었는데, 이번엔 **역할별로 나눠서 각자 함수로 분리하고, 각자 다른 위치/크기의 창**으로 배치했어요.

## 2. 새로 배운 함수 3가지

**① `SameLine()`** — 다음 위젯을 줄바꿈 없이 바로 옆에 붙임.
```cpp
if (ImGui::Button(u8"버튼A")) {}
ImGui::SameLine();
if (ImGui::Button(u8"버튼B")) {}
```

**② `SetNextWindowPos` / `SetNextWindowSize`** — 다음 `Begin()`이 처음 생성될 때만 이 위치/크기로 시작.
```cpp
ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiCond_FirstUseEver);
ImGui::SetNextWindowSize(ImVec2(320, 220), ImGuiCond_FirstUseEver);
ImGui::Begin(u8"그리드");
```
`ImGuiCond_FirstUseEver` 없이 쓰면 매 프레임 강제로 이 위치에 고정돼서 드래그로 못 옮김. 또한 이미 `imgui.ini`에 저장된 위치/크기가 있으면 이 좌표는 무시됨 (테스트할 때 `imgui.ini` 지우고 확인).

**③ `Columns` + `BeginChild`** — 칸으로 나뉜 영역 만들기.
```cpp
ImGui::BeginChild(u8"슬롯영역", ImVec2(0, 140), true);   // 스크롤 가능한 하위 영역
ImGui::Columns(3, u8"슬롯칼럼", false);                   // 그 영역을 3칸으로 분할
for (...) {
    if (ImGui::Button(SlotNames[i], ...))
        m_iSelectedSlot = i;
    ImGui::NextColumn();
}
ImGui::Columns(1);   // 꼭 닫아야 함 - 안 닫으면 이후 위젯도 계속 3칸으로 나뉨
```
세 개가 각자 다른 레벨을 담당: `SetNextWindowPos/Size` = 창 자체 배치, `BeginChild` = 창 안의 스크롤 가능한 하위 영역, `Columns` = 그 영역을 몇 칸으로 나눌지.

## 3. 진짜 창 vs ImGui가 그린 가짜 창

이번에 헷갈렸던 부분 확실히 정리:
- **진짜 Windows 창은 `hWnd` 딱 하나뿐** (Day1에서 `CreateWindow`로 만든 것). 작업표시줄에 뜨고, 알트탭 되는 진짜 OS 창은 이거 하나.
- "툴바"/"설정"/"그리드"는 그 `hWnd` 하나 **안에다 ImGui가 매 프레임 그려주는 그림**일 뿐, 실제 별개의 OS 창이 아님.
- 증거: 이 가짜 창들을 진짜 창 밖으로 드래그해서 못 빼낸다 (기본 ImGui는 진짜 창 영역 안에서만 움직임 — `docking` 브랜치 쓰면 이 제약이 풀림).
- `ImGui::SetNextWindowPos`는 WinAPI `SetWindowPos`(진짜 OS 창을 옮기는 API)랑 이름만 비슷하고 완전히 다른 것 — `hWnd` 없이 그냥 "다음에 그릴 사각형 좌표"만 지정하는 ImGui 자체 함수.

## 4. ImGui는 WinAPI 전용이 아니다

ImGui 코어는 플랫폼을 몰라요. 입력 backend(`imgui_impl_win32` 대신 `imgui_impl_glfw`, `imgui_impl_sdl2` 등)와 그리기 backend(`imgui_impl_dx11` 대신 `imgui_impl_opengl3`, `imgui_impl_vulkan` 등)를 자유롭게 바꿔 끼울 수 있음. 지금 WinAPI+DX11 조합을 쓴 이유는 "이거 아니면 안 돼서"가 아니라, **학습 순서(WinAPI 이미 앎 → ImGui 학습 중 → DX11 예정)에 맞춰서 고른 것** — DX11 배울 때 `Device.cpp`를 그대로 재사용하려는 의도.

## 5. 그리드 ↔ 예전 WinAPI 타일 시스템 대응

예전 WinAPI 프로젝트에서 숫자키 1~9로 타일 전환하던 방식:
```cpp
case '1': m_iCurTile = TILE_GRASS; break;
case '2': m_iCurTile = TILE_WATER; break;
```
→ 화면에 뭐가 몇 번인지 안 보이고, 외우고 있어야 했음.

지금 만든 그리드는 그걸 **"눈에 보이게, 클릭 가능하게"** 바꾼 버전:
```cpp
if (ImGui::Button(SlotNames[i], ...))
    m_iSelectedSlot = i;   // 클릭한 칸 번호 저장 (예전의 m_iCurTile 대응)
```
나중에 실제 맵툴로 발전시키면:
- 버튼 텍스트 자리에 실제 타일 썸네일 **이미지**가 들어감 (`ImGui::Image`, Day7 예정)
- 클릭 시 저장되는 `m_iSelectedSlot` 값이 예전 `m_iCurTile`과 같은 역할
- 그 값 참고해서 맵 클릭 시 타일 찍는 로직은 예전 WinAPI 코드 그대로 재사용 가능
- 숫자키 9개 제한과 달리, 그리드는 버튼 개수 자유롭게 늘리고 스크롤도 되니 타일 종류 많아져도 대응 가능

같은 원리로 "툴바"는 저장/불러오기/모드전환 같은 버튼 모음 자리, "그리드"는 타일 팔레트나 인벤토리처럼 칸으로 골라 쓰는 UI 자리 — 지금 이름 그대로 쓰는 게 아니라 이 배치 기법을 실제 용도에 맞게 재활용하는 것.

## Day5 예고
지금까지 배운 위젯들을 실제 게임 데이터(스탯, 아이템 등)와 연결해서 실시간으로 값을 주고받는 걸 다룰 예정.
