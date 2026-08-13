# DX11 FrameWork 프로젝트 진행 정리

## 프로젝트 개요
- 학원 "12월7일 1일차 FrameWork" 자료 기반 DX11 커리큘럼 진행 중
- VS2022 사용 (원본은 v140 툴셋 / Windows 8.1 SDK, VS2015 기준으로 제작됨)
- 구조: **Engine(DLL 프로젝트)** + **Client(exe 프로젝트)**, 각각 `default`(vcxproj 위치) / `private`(.cpp) / `public`(.h) / `bin`(빌드 결과물) 폴더로 분리
- 원본 학원 자료 구조를 최대한 그대로 따라가는 방식으로 진행 (파일 생성 시 클래식 "새 항목 추가" 대화상자의 위치 필드에 경로를 직접 지정)

---

## 1. 프로젝트/폴더 구조 개념

### Engine과 Client의 역할
- **Engine (DLL)**: 게임 내용과 무관한 범용 저수준 시스템 (렌더링, 리소스 관리, 입력, 수학 라이브러리 등). 재사용 가능해야 하는 코드.
- **Client (exe)**: Engine을 가져다 써서 실제 게임 로직/콘텐츠를 구현하는 실행 파일.
- 언리얼(`UObject`)/유니티도 근본적으로 같은 Engine/Client 분리 구조를 훨씬 크게 발전시킨 것.

### public/private 폴더 분리 이유
- `public`(헤더 .h): 외부에 공개할 인터페이스만 노출
- `private`(구현 .cpp): 실제 구현 코드, 외부에서 감춤
- 목적: 캡슐화(내부 구현을 자유롭게 바꿔도 외부 코드에 영향 없음), DLL 배포 시 실제 소스 코드 자체를 감출 수 있음, 컴파일 시간 단축
- 이 방식은 다소 오래된 관습이며, 최신 실무는 CMake + `src`/`include` 표준 레이아웃을 더 많이 사용. 다만 헤더-구현 분리, 모듈 분리라는 개념 자체는 여전히 기본기로 유효함.

### 필터(가상 폴더) vs 물리적 폴더
- 솔루션 탐색기에 보이는 트리(필터, `.vcxproj.filters`)와 실제 디스크 상의 파일 위치(`.vcxproj`의 Include 경로)는 서로 독립적인 개념
- 원본은 물리적으로는 `public`/`private`에 파일을 나누면서도, 탐색기 트리에서는 `01.Utility\99.SuperClass\Base`처럼 완전히 다른 이름으로 재분류해서 보여줌
- Client의 리소스 관련 파일(.rc, .ico, Resource.h 등)도 "리소스 파일" 기본 필터 대신 `98.Default`라는 커스텀 필터로 재정리되어 있었음

---

## 2. 파일 생성 시 경로 지정 방법 (원본 방식 그대로 따라가기)

VS2022 기본 "새 항목 추가" 창은 파일명만 입력하게 되어 있어 `\`, `/` 자체가 금지 문자로 막혀 있음. 원본처럼 만들려면:

1. 프로젝트 우클릭 → 추가 → 새 항목
2. **"모든 템플릿 표시(T)"** 클릭 → 클래식 대화상자로 전환
3. 템플릿 선택 (헤더는 **헤더 파일(h)**, 구현은 **C++ 파일(cpp)**)
4. 하단 입력란
   - **이름(N)**: `Base.h` / `Base.cpp`
   - **위치(L)**: 예) `...\Engine\public`, `...\Engine\private`
5. 추가(A) 클릭

- 이 방법은 파일 생성 시점마다 매번 위치를 지정해야 함 (폴더를 미리 만들고 그 안에서 추가하는 방식은 원본과 다르므로 사용하지 않기로 함)
- Windows 탐색기로 파일을 직접 옮기면 VS가 자동 인식 못함 (vcxproj에 경로가 텍스트로 박혀있는 방식이라 어긋남) → 반드시 생성 시점에 위치 지정하거나, 옮긴 후 "제거 후 기존 항목으로 재추가" 필요

### 추가로 직접 챙겨야 하는 것
- **`#pragma once`**: 헤더 파일(h) 템플릿은 빈 파일로 생성되므로 직접 작성 필요
- **include 문**: Engine은 `AdditionalIncludeDirectories` 설정이 없어 `#include "..\public\Base.h"`처럼 상대경로 직접 명시 필요. Client는 자기 자신의 public 폴더(`../public/`)만 등록되어 있어 자기 헤더는 파일명만으로 include 가능.

---

## 3. 프로젝트 속성 설정

### 출력 디렉터리 (OutDir)
- 원본: Engine/Client 둘 다 **x64 플랫폼에만** `OutDir = ..\bin\` 설정 (Win32는 미설정)
- 설정 위치: 프로젝트 속성 → 구성 속성 → **일반**(C/C++ 하위 "일반"이 아님, 최상위 "일반") → 출력 디렉터리
- **주의**: 향후 Win32 플랫폼을 사용하게 되면 OutDir(`..\bin\`) 설정을 Win32 구성에도 추가해줘야 함

### 중간 디렉터리 (IntDir)
- 원본은 별도 지정 없이 기본값(`$(Platform)\$(Configuration)\`, 프로젝트 파일 기준) 사용 → 결과적으로 `default` 폴더 안에 `x64\Debug` 생성
- VS2022 최신 템플릿은 기본값이 `$(SolutionDir)$(Platform)\$(Configuration)\`(솔루션 기준)인 경우가 많아, 그대로 두면 중간 산출물이 `default` 밖(상위 폴더)에 생김 → 원본과 똑같이 맞추려면 명시적으로 프로젝트 파일 기준 경로로 재설정 필요

### 추가 포함 디렉터리
- Client: `../public/` (자기 자신의 public 헤더용)
- Engine의 Base.h를 Client에서 쓰려면 `..\..\Engine\public` 같은 경로 추가 필요

### PlatformToolset / SDK 버전
- 원본: `WindowsTargetPlatformVersion = 8.1`, `PlatformToolset = v140`
- VS2022 기본은 v143 + 최신 SDK. v140 개별 구성요소를 설치해 원본 그대로 쓰거나, 솔루션 재대상(Retarget)으로 v143에 맞춰 진행 가능 (재대상 시 옛 코드에서 경고/에러 가능성 있음)

---

## 4. 겪었던 문제와 해결 (트러블슈팅 로그)

### 4-1. 템플릿 선택 오류 정정
- 처음에 Client를 "빈 프로젝트"로 만들어도 된다고 안내했었으나, 원본 `Client.cpp`를 실제로 확인해보니 `About` 대화상자, `MAX_LOADSTRING`, 리소스 로드 방식 등 전형적인 템플릿 생성 코드가 포함되어 있어 **정정**: Client는 **"Windows 데스크톱 애플리케이션" 템플릿**으로 생성해야 원본과 동일한 구조가 나옴. (Engine은 "동적 라이브러리(DLL)" 템플릿이 맞음)

### 4-2. 프로젝트 파일 경로 꼬임 발견 → 처음부터 재작업 결정
1차 시도에서 발견된 문제들:
- Engine 프로젝트 파일이 두 개 존재 (`Engine\Engine.vcxproj`가 실제 솔루션이 참조하는 파일, `Engine\default\Engine.vcxproj`는 설정만 해놓은 죽은 파일)
- 실제 사용 중인 Engine.vcxproj의 `ConfigurationType`이 DLL이 아닌 `Application`(콘솔 exe)으로 잘못 설정됨
- Client의 MainApp.cpp/h, Client_Defines.h가 vcxproj에 전혀 등록되지 않음 (물리적으로만 존재)
→ 개별 수정보다 **Engine/Client 프로젝트를 처음부터 다시 생성**하기로 결정

### 4-3. 재생성 시 폴더 한 단계 더 깊어지는 문제
- 기존 솔루션에 새 프로젝트를 추가할 때는 "솔루션과 같은 디렉터리에 배치" 체크박스가 없어서, 위치(`...\Engine\default`) + 이름(`Engine`) 조합으로 폴더가 한 단계 더 깊게 생성됨 (`Engine\default\Engine\`)
- **해결**: VS 닫고 파일들을 한 단계 위로 이동 → `FrameWork.sln`을 텍스트 에디터로 열어 프로젝트 경로 문자열 직접 수정 → 다시 열기

### 4-4. PCH(미리 컴파일된 헤더) 파일명 혼동
- **Engine**(DLL 템플릿) → PCH 파일명이 `pch.h`
- **Client**(Windows 데스크톱 앱 템플릿) → PCH 파일명이 `framework.h`
- 서로 다른 이름을 헷갈려서 처음에 잘못 안내했던 부분 → 각 프로젝트에 맞는 이름으로 정정

### 4-5. PCH include 문자열이 정확히 일치해야 함
- `#include "..\default\pch.h"`처럼 경로를 붙이면, 컴파일러가 프로젝트 설정에 적힌 `pch.h` 문자열과 다르다고 인식해 PCH를 못 찾는 에러(`미리 컴파일된 헤더를 찾는 동안 예기치 않은 파일의 끝...`)가 계속 발생
- **해결**: 추가 포함 디렉터리에 `..\default\`를 등록하고, 소스에서는 `#include "pch.h"`처럼 파일명만 짧게 쓰는 방식으로 변경

### 4-6. 1일차 원본 자체의 미완성 코드
2일차 자료와 대조하여 확인한 결과, 아래는 원본 자체의 미완성/오류였음:
- `MainApp.cpp`의 `delete m_iData;` — `MainApp.h`에 해당 멤버 변수 선언 자체가 없음. 2일차 자료에서 `Free()`가 완전히 빈 함수로 바뀌며 이 코드는 삭제됨 → **삭제 처리**
- `MainApp.h`의 `virtual void Free() override;`, `MainApp.cpp`의 `__super::Free();` — `CMainApp`이 1일차 시점엔 아무 것도 상속받지 않은 상태라 "기본 클래스 멤버가 아님" 에러 발생. 2일차에서 `class CMainApp final : public CBase`로 바뀌며 정상적으로 쓰이게 됨 → **현재는 주석 처리하고 진행**

---

## 5. 현재 상태
✅ 솔루션 정리(Clean) → 솔루션 다시 빌드(Rebuild) 결과 **Engine, Client 2개 프로젝트 모두 빌드 성공** 확인 완료

## 6. 다음 단계 예고 (2일차 자료 기준)
1일차를 마저 진행한 뒤 2일차로 넘어가면 다음 내용들이 등장함:
- `CMainApp`이 `CBase`를 상속 (`class CMainApp final : public CBase`) — 지금 주석 처리해둔 `override`/`__super::Free()`가 정상적으로 살아남
- `BEGIN(Client)` / `END` 매크로 (namespace를 매크로로 감싸는 방식)
- `Safe_Release` 매크로/함수 등장
- `Reference` 폴더(`Headers/`, `Librarys/Engine.lib`) + `UpdateLib.bat` — Engine을 미리 빌드해 헤더와 .lib만 뽑아 Client가 참조하는 구조로 전환

## 7. 참고 개념 — 참조 카운팅 (CBase)
- `CBase`의 `m_dwRefCnt`, `AddRef()`, `Release()`는 여러 객체가 하나의 리소스(텍스처/메시/사운드 등)를 포인터로 공유할 때, "지금 몇 명이 참조 중인지" 숫자만 세어 0이 되는 순간 자동 삭제(`delete this`)하는 수명 관리 기법
- 싱글톤(인스턴스를 하나로 제한하는 패턴)과는 다른 개념 — 참조 카운팅은 인스턴스 개수 제한이 없고, 이미 존재하는 특정 인스턴스를 여러 곳이 공유할 때의 수명 관리가 목적
- 누가 들고 있는지는 추적 안 하고 "몇 명이 들고 있는지"만 관리 → 실무에서는 스마트 포인터(`shared_ptr`, `ComPtr`)로 자동화하고, weak reference·스레드 안전성·디버그 추적 로그 등 안전장치를 추가해서 사용
- 엔진에서 다루는 이유: 리소스 재사용(메모리 절약)과 안전한 해제 시점 판단을 동시에 만족시키는 가장 단순한 방법이라 공통 부모 클래스(`CBase`)에 기본 탑재
