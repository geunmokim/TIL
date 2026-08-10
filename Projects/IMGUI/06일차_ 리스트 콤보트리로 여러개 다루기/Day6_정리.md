# Day6 정리: 리스트/콤보/트리로 여러 개 다루기

## 1. 핵심 변화 — 몬스터 1마리 → 배열 3마리

```cpp
static const int	MONSTER_COUNT = 3;
MonsterData			m_Monsters[MONSTER_COUNT];
int					m_iSelectedMonster;
```

**배열로 관리하는 이유**: 몬스터가 몇 마리든 `for`문 하나로 순회 가능하고, `ListBox`가 리턴하는 "인덱스"로 바로 `m_Monsters[인덱스]`처럼 접근 가능. 변수를 `m_Monster1`, `m_Monster2`처럼 따로따로 뒀다면 반복문도 불가능하고 "몇 번째"라는 개념 자체가 없었을 것.

(참고: 나중에 개수가 유동적으로 늘어나야 하면 `std::vector<MonsterData>`로 바꾸는 게 나음 — 인덱스 접근이 빠른 `vector`가 `ListBox` 같은 UI와 궁합이 좋음. `std::list`는 노드 기반이라 인덱스 접근이 느려서 이런 용도엔 안 맞음.)

## 2. 새로 배운 위젯 3개

```cpp
// ListBox: 목록에서 클릭해서 고름, 리턴값은 선택된 인덱스
const char* NameList[MONSTER_COUNT];
for (int i = 0; i < MONSTER_COUNT; ++i)
	NameList[i] = m_Monsters[i].Name;
ImGui::ListBox(u8"목록", &m_iSelectedMonster, NameList, MONSTER_COUNT, 4);
//                        선택인덱스(주소)   문자열배열   개수   화면에 보일 줄 수

// Combo: ListBox랑 기능 비슷, 평소엔 접혀있다가 클릭하면 펼쳐지는 드롭다운
ImGui::Combo(u8"종족", &m_iMonsterType, TypeNames, IM_ARRAYSIZE(TypeNames));

// TreeNode: 클릭하면 접혔다 펼쳐지는 그룹 영역
if (ImGui::TreeNode(u8"색상 상세 설정"))
{
	ImGui::ColorEdit3(u8"색상", Selected.Color);
	ImGui::TreePop();   // TreeNode가 true였을 때만 반드시 짝으로 호출
}
```

리스트에서 선택된 항목은 **참조**로 받아서 바로 수정되게 함:
```cpp
MonsterData& Selected = m_Monsters[m_iSelectedMonster];
```

## 3. `-1` 초기값의 의미

```cpp
m_iSelectedSlot(-1)
```
배열 인덱스(`0`~`5`)는 전부 "진짜 칸"으로 이미 쓰이고 있어서, "아직 아무것도 선택 안 됨"을 표현하려면 그 범위 밖의 값이 필요함. 인덱스는 원래 음수가 될 수 없으니 `-1`을 "선택 없음" 신호로 관례적으로 씀.
```cpp
if (m_iSelectedSlot >= 0) ... // 진짜 선택된 상태
else ...                       // -1이면 "선택 없음" 처리
```
(참고: `m_iSelectedMonster`는 `0`으로 초기화해도 괜찮았음 — 몬스터 배열엔 항상 3마리가 있어서 "0번째"가 처음부터 유효한 선택이었기 때문. 그리드 슬롯은 "아직 아무것도 안 눌렀다"는 상태가 있을 수 있어서 `-1`이 필요했던 것.)

## 4. `#define` vs `static const int` 기준 정리

| 용도 | 선택 |
|---|---|
| 숫자/값 상수 (배열 크기, 최대값 등) | `static const int` — 타입 있음, 디버거에 보임, 클래스 스코프 소속 |
| 여러 줄 코드를 통째로 찍어내는 매크로 (`DECLARE_SINGLETON` 등) | `#define` |
| 컴파일 자체를 켜고 끄는 것 (`#ifdef _DEBUG`) | `#define`/전처리기 — `const`로는 대체 불가 |

기준은 "수정할 일이 있냐 없냐"가 아니라 **"값이냐, 코드/컴파일 여부냐"**.

## 5. ImGui vs MFC — 용도가 다른 도구라는 것

MFC가 밀리는 건 ImGui 때문이 아니라 Windows 데스크톱 개발 흐름 자체가 WPF/UWP/WinUI 등으로 넘어가서. ImGui와 MFC는애초에 자리가 다름:

| | 용도 |
|---|---|
| MFC, WPF, Qt | 정식 출시하는 프로그램의 완성된 UI |
| ImGui | 개발 중 빠르게 만드는 디버그 툴, 게임 인게임 오버레이, 사내 툴 |

**나중에 DX11로 모작 만들 때 ImGui의 역할**: 예전 WinAPI에서 하던 "마우스 클릭 → 타일 좌표 변환 → 배열에 타일 기록" 로직은 그대로 재사용. ImGui는 그 위에 얹혀서 "지금 어떤 타일이 선택됐는지 보여주고 고르게 해주는 팔레트 UI" 역할만 새로 담당하는 구조.
```
[ImGui 팔레트 창] 클릭해서 타일 선택 → 변수 갱신
                                          ↓
[실제 맵 화면 (DX11 렌더링)] 마우스 클릭 → 그 변수 값으로 타일 교체 (예전 로직 재사용)
```

## Day7 예고
지금 그리드의 "칸1~칸6" 텍스트 자리에 실제 이미지(`ImGui::Image`)를 넣는 법을 배울 예정 — 위 구조에서 팔레트에 실제 타일 썸네일이 보이게 되는 단계.
