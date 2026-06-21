# WinAPI 게임 개발 학습 노트

## 프로젝트 구조 설계 순서

```
1단계: 기초 헤더
Define.h    → 매크로, 상수 정의 (WINCX, WINCY 등)
Function.h  → 템플릿 함수 (Safe_Delete, SAFE_DELETE 등)
Struct.h    → 구조체 정의 (INFO 등)
Extern.h    → 전역변수 extern 선언
Include.h   → 위 헤더들 한 곳에 모아서 include
framework.h → STL, windows.h 등 공통 헤더 모음

2단계: 핵심 관리 클래스
GameManager  → 게임 전체 흐름 관리 (싱글톤)

3단계: Scene 시스템
Scene        → 화면 단위 (로고, 타이틀, 게임화면)
SceneManager → Scene 전환 관리

4단계: Object 계층
Obj          → 최상위 부모 (공통 데이터/함수)
Camera
Player
Monster
```

전체 흐름:
```
GameManager
    └── SceneManager
            ├── LogoScene
            ├── TitleScene
            └── GameScene
                    └── ObjManager
                            ├── Player
                            ├── Monster
                            └── Camera
```

---

## WinAPI 게임 루프

VS 최신 템플릿은 게임용으로 바로 쓸 수 없음 → 아래와 같이 수정 필요

```cpp
// ❌ 기본 템플릿 (일반 윈도우 앱용, 게임에 부적합)
while (GetMessage(&msg, nullptr, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    // Update, Render 없음! → 화면에 아무것도 안 그려짐
}

// ✅ 게임용 루프
CMainGame mainGame;
mainGame.Initialize();

msg.message = WM_NULL;
DWORD dwTime = GetTickCount();

while (WM_QUIT != msg.message)
{
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (dwTime + 10 < GetTickCount())
    {
        mainGame.Update();
        mainGame.Render();
        dwTime = GetTickCount();
    }
}
```

GetMessage vs PeekMessage 차이:
- `GetMessage` → 메시지 올 때까지 기다림 → Update/Render 안 돌아감
- `PeekMessage` → 메시지 없어도 계속 진행 → Update/Render 매 프레임 돌아감
- 게임은 반드시 `PeekMessage` 사용

---

## VS 버전별 차이 (VS2022 기준)

최신 템플릿에서 빠진 것들 → 직접 추가해야 함:

```cpp
// 1. 전역변수 선언 (메인 cpp에)
HWND g_hWnd;

// 2. extern 선언 (Extern.h에)
extern HWND g_hWnd;

// 3. InitInstance 함수 끝에
g_hWnd = hWnd;

// 4. 게임 루프 (PeekMessage 방식으로 교체)
// 5. CMainGame 생성 및 Initialize/Update/Render 호출
```

---

## 화면 좌표계

WinAPI는 수학 좌표계가 아닌 화면 좌표계를 사용

```
수학 좌표계          화면(WinAPI) 좌표계
     +Y              (0,0)----→ +X
      ↑                |
      |                ↓
------+→ +X           +Y
```

Y축이 반대라서:
```cpp
m_tInfo.fY -= m_fSpeed;  // Y 감소 → 화면에서 위로 이동
m_tInfo.fY += m_fSpeed;  // Y 증가 → 화면에서 아래로 이동
```

화면 좌표계 사용하는 것들:
- WinAPI, SDL, SFML, HTML5 Canvas
- 안드로이드/iOS 화면 좌표
- DirectX (스크린 스페이스)
- Unity 내부 렌더링

Y 위 증가 사용하는 것들:
- Unity 3D/2D 월드 좌표
- Unreal Engine
- OpenGL (월드 좌표)

---

## ZeroMemory

```cpp
ZeroMemory(&m_tInfo, sizeof(m_tInfo));
// 지정한 메모리 영역을 전부 0으로 초기화하는 Windows API 매크로
// 내부적으로 memset(ptr, 0, size)와 동일

// 초기화 방법들 (전부 동일한 결과)
ZeroMemory(&rc, sizeof(rc));   // WinAPI 스타일
memset(&rc, 0, sizeof(rc));    // C 스타일
RECT rc = {};                  // C++ 스타일 (제일 간결)
```

생성자에서 사용 예:
```cpp
CObj::CObj()
    : m_fSpeed(0.f)       // float은 이니셜라이저로 초기화
{
    ZeroMemory(&m_tInfo, sizeof(m_tInfo));  // 구조체는 ZeroMemory로 초기화
    ZeroMemory(&m_tRect, sizeof(m_tRect));
}
```

---

## GetAsyncKeyState

```cpp
// "지금 이 순간 Windows에게 키가 눌려있냐?" 실시간으로 물어보는 함수
GetAsyncKeyState(VK_LEFT);  // 왼쪽 화살표
GetAsyncKeyState('W');      // 알파벳은 대문자로! ('w'는 동작 안 함)

// & 0x8000 → 최상위 비트만 확인 (지금 눌려있는지 체크)
if (GetAsyncKeyState('W') & 0x8000)  // 올바른 사용법

// 주의: & 0x8000 위치
if (GetAsyncKeyState(VK_LEFT) & 0x8000)   // ✅ 괄호 밖
if (GetAsyncKeyState(VK_LEFT & 0x8000))   // ❌ 괄호 안 (오타 주의!)
```

가상 키 코드:
```cpp
VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN  // 방향키
VK_SPACE  // 스페이스바
VK_ESCAPE // ESC
'W' == 0x57, 'A' == 0x41, 'S' == 0x53, 'D' == 0x44  // 알파벳은 ASCII = 가상키코드
```

---

## 오브젝트 설계 원칙

### CObj 계층 구조

```cpp
CObj        → 공통 데이터 + 공통 함수 제공 (뼈대)
              fX, fY, iCX, iCY, fSpeed
              Update_Rect(), Set_Pos()  → 모든 오브젝트 동일 → 일반 함수
              Update(), Render()        → 오브젝트마다 다름  → virtual 함수

CPlayer     → 플레이어 고유 동작 (키입력, 총알 생성)
CBullet     → 총알 고유 동작 (직진)
CEnemy      → 적 고유 동작 (AI)

CMainGame   → 전체 흐름 관리
              오브젝트 생성/삭제, 리스트 관리, Update/Render 순서 제어
```

### virtual vs 일반 함수

```
virtual 함수  → 오브젝트마다 동작이 다를 때
              예: Update(), Render(), OnCollide()
              
일반 함수     → 모든 오브젝트가 동작이 같을 때
              예: Update_Rect(), Set_Pos(), Get_Rect()
              이유: virtual은 vtable 조회 오버헤드 발생
                   매 프레임 모든 오브젝트가 호출하는 함수에 불필요한 virtual은 성능 낭비
```

### 단일 책임 원칙 (SRP)

```
총알을 쏘는 행위   → CPlayer 책임
총알이 날아가는 것 → CBullet 책임
총알 전체 관리     → CMainGame 책임
```

### 오브젝트 생성 원칙

```
핵심 질문: "이 오브젝트가 어디서 태어나야 하는지 누가 알고 있냐?"
그 정보를 가진 쪽이 생성 책임을 갖는다

총알      → 플레이어 위치를 아는 CPlayer가 생성
아이템    → 몬스터 위치를 아는 CEnemy가 생성
이펙트    → 충돌 위치를 아는 충돌 매니저가 생성
```

순환 참조 문제:
```cpp
// CBullet에서 총알 생성하면?
#include "Player.h"  // Player가 Bullet 포함
                     // Bullet이 Player 포함 → 순환 참조 에러!
// 그래서 Player가 총알 생성 책임을 가짐
```

---

## 컨테이너 선택 기준

```
list   → 생성/삭제가 빈번한 오브젝트 (총알, 몬스터, 이펙트)
         삭제 시 연결만 바꾸면 됨 → O(1)

vector → 인덱스 접근이 필요하거나 삭제가 드문 것 (타일맵, 웨이포인트, 프레임)
         삭제 시 뒤를 당겨야 함 → O(n)

map    → 키로 검색이 필요한 것 (리소스 관리)
```

---

## 총알 발사 구조

```
CMainGame : m_listBullet 원본 소유 (전체 관리 - Update/Render/삭제)
CPlayer   : m_pBullet 포인터로 주소만 받아서 총알 추가만 함
CBullet   : 날아가는 것만 담당
```

```cpp
// MainGame.cpp
static_cast<CPlayer*>(m_pPlayer)->Set_Bullet(&m_listBullet);
// → 원본 주소를 Player에게 전달

// Player.cpp
if (GetAsyncKeyState('W') & 0x8000)
{
    My_AsyncKeyState('W');
    m_pBullet->emplace_back(Create_Bullet());  // MainGame의 리스트에 직접 추가
}

// Create_Bullet() - Player에서 생성하는 이유: 플레이어 위치 정보가 여기 있음
CObj* CPlayer::Create_Bullet()
{
    CObj* pObj = new CBullet;
    pObj->Initialize();
    pObj->Set_Pos(m_tInfo.fX, m_tInfo.fY);  // 플레이어 위치에서 생성
    static_cast<CBullet*>(pObj)->My_AsyncKeyState(m_Key);
    return pObj;
}
```

list를 `CObj*`로 관리하는 이유 (다형성):
```cpp
// ✅ CObj*로 관리 → 총알 종류 늘어나도 리스트 하나로 관리
list<CObj*> m_listBullet;
m_listBullet.emplace_back(new CBullet());
m_listBullet.emplace_back(new CFireBullet());  // 그냥 추가
m_listBullet.emplace_back(new CIceBullet());   // 그냥 추가

// 반복문 하나로 전부 처리 (virtual 덕분에 각자 동작)
for (auto& pBullet : m_listBullet)
    pBullet->Update();

// ❌ CBullet*로 관리 → 종류마다 리스트 따로 만들어야 함
list<CBullet*>    m_pBullet;
list<CFireBullet*> m_pFireBullet;  // 리스트 추가
list<CIceBullet*>  m_pIceBullet;   // 리스트 추가
```

---

## 잔상 제거 원리

```cpp
void CMainGame::Render()
{
    Rectangle(m_hDC, 0, 0, WINCX, WINCY);  // 매 프레임 흰 사각형으로 전체 덮기
    m_pPlayer->Render(m_hDC);               // 그 위에 다시 그리기
    for (auto& pBullet : m_listBullet)
        pBullet->Render(m_hDC);
}
// WINCX, WINCY 범위 밖은 덮이지 않음 → 총알 삭제 코드 별도로 필요
```

---

## 헝가리안 표기법 (WinAPI 스타일)

```cpp
m_listBullet  // m_ = 멤버변수, list = list 타입
m_pBullet     // m_ = 멤버변수, p   = 포인터
m_tInfo       // m_ = 멤버변수, t   = 구조체(type)
m_fSpeed      // m_ = 멤버변수, f   = float
m_iCount      // m_ = 멤버변수, i   = int
m_hDC         // m_ = 멤버변수, h   = 핸들(handle)
```

---

## SOLID 원칙 (게임에서 중요한 것)

```
S - Single Responsibility Principle (단일 책임 원칙) ← 가장 중요
    각 클래스는 자기 역할 하나만 잘 하면 된다

O - Open/Closed Principle (개방 폐쇄 원칙)
L - Liskov Substitution Principle (리스코프 치환 원칙)
I - Interface Segregation Principle (인터페이스 분리 원칙)
D - Dependency Inversion Principle (의존성 역전 원칙)
```

---

## 현재 완성 현황 & 다음 단계

```
✅ 완료
- 게임 루프 (PeekMessage 방식)
- 플레이어 이동 (방향키)
- 총알 발사 (WASD)
- 화면 잔상 제거

📌 다음 단계
- 화면 밖 총알 삭제 (remove_if 활용)
- 적(Enemy) 구현
- 충돌 처리
- 타일맵
```
