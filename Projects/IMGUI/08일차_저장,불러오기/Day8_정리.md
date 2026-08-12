# Day8 정리: 저장/불러오기

## 1. 새로 추가된 것

| 항목 | 역할 |
|---|---|
| `GetSaveFileName` / `GetOpenFileName` | Win32 표준 파일 대화상자 — 예전 프로젝트에서도 써봤을 익숙한 API |
| `Save_Monsters` / `Load_Monsters` (새 함수) | 대화상자로 경로 받고, `fwrite`/`fread`로 구조체 배열을 통째로 저장/복원 |
| 툴바 버튼 실제 기능 연결 | "버튼A/B/C" → "저장"/"불러오기"/"초기화"로 라벨 변경 + 기능 연결 |

## 2. 저장/불러오기 핵심 코드

```cpp
// 저장: 구조체 배열을 있는 그대로 바이너리로 기록
fwrite(m_Monsters, sizeof(MonsterData), MONSTER_COUNT, pFile);

// 불러오기: 파일에서 그대로 읽어와 배열에 덮어씀
fread(m_Monsters, sizeof(MonsterData), MONSTER_COUNT, pFile);
```
`.dat` 파일은 사람이 메모장으로 읽을 수 없는 **바이너리 형태**(구조체 메모리를 그대로 파일에 기록) — 프로그램만 다시 읽으면 되는 용도라 정상.

## 3. 이번에 있었던 실수 — `g_hWnd`는 존재한 적 없는 변수였음

파일 대화상자의 `ofn.hwndOwner`에 창 핸들을 넘겨야 하는데, 처음에 `g_hWnd`라는 전역변수가 `Window.cpp`/`Define.h`에 있다고 잘못 안내함 (이 Day1~8 시리즈에선 `hWnd`를 계속 `WinMain`의 지역변수로만 썼지, 전역변수로 만든 적이 없었음). 바로잡은 방법:

```cpp
// MainGame.h
private:
	HWND	m_hWnd;		// 파일 대화상자에 넘길 창 핸들 저장용

// MainGame.cpp - Init_ImGui 맨 위
HRESULT CMainGame::Init_ImGui(HWND hWnd)
{
	m_hWnd = hWnd;		// 인자로 받은 값을 멤버에 저장해뒀다가 나중에 재사용
	...

// Save_Monsters / Load_Monsters
ofn.hwndOwner = m_hWnd;
```
전역변수 새로 안 만들고, 이미 `Init_ImGui`가 받은 `hWnd`를 멤버 변수에 저장해뒀다가 재사용하는 방식으로 해결.

## 4. `SameLine()` 복습
Day4에서 배운 걸 여기서도 그대로 재사용 — 버튼 사이에 안 넣으면 각 버튼이 세로로 쌓임. ImGui 위젯은 기본적으로 하나 그리면 자동 줄바꿈되는 게 기본 동작이라, 가로 배치하려면 매번 명시적으로 `SameLine()` 필요.

## 확인한 것
저장 → 몬스터 값(이름/HP 등) 변경 → 불러오기로 저장 시점 값으로 복원되는 사이클 정상 작동 확인.

## Day9 예고 (마지막)
지금까지 배운 걸 전부 합쳐서 **미니 맵툴**을 만들 예정 — 그리드(Day6)를 타일 팔레트로, 이미지(Day7)로 실제 타일 썸네일 표시, 저장/불러오기(Day8)로 맵 데이터 영속화. Day1~8에서 따로따로 연습했던 조각들을 하나의 완성된 흐름으로 조립하는 단계.
