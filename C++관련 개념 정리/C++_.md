# C++ 핵심 개념 정리

---

## 1. 메모리 초기화 - memset()

메모리를 특정 값으로 한번에 채우는 함수. `#include <cstring>` 필요.

```cpp
memset(주소, 값, 바이트크기);

int arr[10];
memset(arr, 0, sizeof(arr));       // 배열 0으로 초기화

char str[100];
memset(str, 0, sizeof(str));       // char 배열 초기화

INFO info;
memset(&info, 0, sizeof(INFO));    // 구조체 초기화
```

> ⚠️ **주의**: memset은 바이트 단위로 채운다.
> `memset(arr, 1, sizeof(arr))` → 각 바이트가 0x01이 되어 int값은 0x01010101이 됨. 숫자 1이 아님!

---

## 2. 비트 연산

```cpp
x >>= 1;   // x = x / 2 와 동일 (오른쪽 시프트)
x <<= 1;   // x = x * 2 와 동일 (왼쪽 시프트)
```

---

## 3. 난수 - rand()

```cpp
rand() % 50;   // 0~49 범위의 난수

// 매번 다른 난수를 원할 경우 srand() 사용
srand(unsigned(time(NULL)));   // main 또는 초기화 시 한 번만 호출
int n = rand() % 50;
```

> `srand()`를 생성자 안에 넣으면 객체 생성마다 호출되어 문제가 생길 수 있음.
> 초기화 함수에서 한 번만 호출하는 것이 올바른 방식.

---

## 4. 포인터와 참조

```cpp
.   // 객체(실체)에 접근
->  // 포인터가 가리키는 객체에 접근

int a = 10;
int* p = &a;    // &는 주소 얻기
*p = 20;        // *는 원본에 접근 (역참조)

int& ref = a;   // &는 별명(참조)으로 사용
```

---

## 5. 이니셜라이저 (초기화 리스트)

메모리 할당 시점에 값을 채워 넣는 것. 선언과 동시에 초기화 효과.

```cpp
class CObj
{
public:
    CObj() : m_iNormal(0), m_iConst(0) {}
    CObj(int _n) : m_iNormal(_n), m_iConst(_n) {}
private:
    int       m_iNormal;
    const int m_iConst;   // const 멤버는 이니셜라이저로만 초기화 가능
};
```

| 구분 | 생성자 | 이니셜라이저 |
|------|--------|-------------|
| 초기화 시점 | 변수 선언 후 초기화 | 선언과 동시에 초기화 |
| const 멤버 | 불가능 | 가능 |

> ⚠️ 초기화 순서는 이니셜라이저 목록 순서가 아닌 **클래스 내 선언 순서**를 따름.

---

## 6. const 객체

```cpp
const CObj obj2;
obj2.Const_Func();    // 가능 - const 멤버 함수만 호출 가능
obj2.Normal_Func();   // 불가능
```

- 모든 멤버 값 변경 불가
- const 멤버 함수만 호출 가능
- 이니셜라이저를 이용한 초기화는 가능

---

## 7. static

```cpp
// static 멤버 변수: 해당 클래스로 만든 모든 객체가 공유
class CObj
{
    static int m_iCount;   // 전역에서 초기화 필요
};
int CObj::m_iCount = 0;
```

- 프로그램 실행 시 메모리에 등록 (생성자 호출보다 먼저)
- 모든 객체가 하나의 static 변수를 공유

---

## 8. 복사 생성자

이미 존재하는 객체를 이용해서 새 객체를 초기화할 때 호출.

```cpp
Person p1("Alice");
Person p2 = p1;    // 복사 생성자 호출 (대입처럼 보이지만 생성자 호출)
```

**호출되는 경우**
1. 객체 초기화: `Person p2 = p1;`
2. 함수 인자를 값으로 전달: `void func(Person p) {}`
3. 함수가 객체를 값으로 반환할 때

---

## 9. 얕은 복사 vs 깊은 복사

### 얕은 복사 (Shallow Copy)
포인터 멤버가 있을 때 주소만 복사 → 같은 메모리를 공유하게 됨 → 소멸 시 중복 delete로 크래시 발생

```cpp
// 얕은 복사 문제
Data a(10);
Data b = a;    // b.value와 a.value가 같은 주소를 가리킴
// a, b 소멸 시 같은 메모리를 두 번 delete → 크래시!
```

### 깊은 복사 (Deep Copy)
복사 생성자를 직접 정의해서 새 메모리를 할당하고 값을 복사

```cpp
Data(const Data& other)
{
    this->value = new int(*other.value);   // 새 메모리 할당 후 값 복사
}
```

**얕은 복사가 자동으로 발생하는 경우**
- 컴파일러가 자동 생성하는 복사 생성자
- 컴파일러가 자동 생성하는 복사 대입 연산자

---

## 10. this 포인터

현재 객체가 할당된 메모리 시작 주소를 가리키는 포인터.

```cpp
void Func()
{
    cout << this << endl;   // 현재 객체의 주소 출력
}
```

> `this == nullptr` → 객체 자체가 없음 (디버깅 시 참고)

---

## 11. 복사 생성자 vs 대입 연산자

```cpp
CObj obj1(10, 20);   // 인자 있는 생성자
CObj obj2 = obj1;   // 복사 생성자 (객체 생성 시)
CObj obj3;          // 기본 생성자
obj3 = obj1;        // 대입 연산자 (객체 생성 후)
```

---

## 12. 상속

```cpp
class CPlayer : public CObj   // public 상속 (private가 기본값이므로 명시 필요)
{
};
```

**상속 시 접근 지정자**

| 부모 멤버 | public 상속 | protected 상속 | private 상속 |
|----------|------------|---------------|-------------|
| public | public | protected | private |
| protected | protected | protected | private |
| private | 접근 불가 | 접근 불가 | 접근 불가 |

**상속 관계**
- is-a 관계: 상속 사용 (`CPlayer`는 `CObj`이다)
- has-a 관계: 멤버로 추가 (상속 X)

---

## 13. 객체 포인터 & 업캐스팅

```cpp
CObj* pTemp = new CPlayer;   // 가능 (업캐스팅)
// CPlayer* p = new CObj;   // 불가능 (다운캐스팅은 명시적으로)

// 객체 포인터는 타입 기준으로 멤버에 접근
pTemp->CObj_Func();          // 가능
// pTemp->CPlayer_Func();   // 불가능 (CObj 타입 기준)
```

---

## 14. 오버라이딩

상속 관계에서 부모 클래스의 함수를 자식 클래스에 재정의. 반환 타입, 함수이름, 매개변수가 모두 동일해야 함.

```cpp
// virtual 없을 때 → 포인터 타입 기준 호출
CObj* pObj = new CPlayer;
pObj->Func();   // CObj::Func() 호출

// virtual 있을 때 → 실제 객체 기준 호출 (동적 바인딩)
pObj->Func();   // CPlayer::Func() 호출
```

---

## 15. virtual (가상 함수)

```cpp
class CObj
{
public:
    virtual void Func() { }   // 가상 함수
    virtual ~CObj() { }       // 소멸자도 virtual로 선언 (메모리 누수 방지)
};
```

| 구분 | 호출 기준 |
|------|---------|
| 일반 함수 | 포인터 타입 (정적 바인딩) |
| virtual 함수 | 실제 객체 타입 (동적 바인딩) |

> ⚠️ 부모 클래스에 virtual 함수가 있으면 **소멸자도 virtual**로 만들어야 자식 소멸자까지 정상 호출됨.

**가상 함수 테이블 (vtable)**
- 가상 함수들의 주소를 저장하는 포인터 배열
- virtual 키워드가 하나라도 있으면 객체에 가상 함수 포인터(4bytes) 추가

---

## 16. 순수 가상 함수 & 추상 클래스

```cpp
class CObj
{
public:
    virtual void Func() = 0;   // 순수 가상 함수
};
```

- 정의부가 없는 가상 함수
- 자식 클래스가 반드시 오버라이딩하도록 강제
- 순수 가상 함수를 가진 클래스 = **추상 클래스** → 객체 생성 불가 (포인터는 가능)
- 자식이 오버라이딩하지 않으면 자식도 추상 클래스가 됨

---

## 17. 캐스팅

```cpp
// C 기반 (비권장)
int iA = (int)3.14;   // 강제 형변환, 타입 검사 없음

// C++ 기반
int iA = static_cast<int>(3.14);
```

### static_cast
- 컴파일 시점에 형변환 결정
- 상속 관계만 확인 (부모-자식 방향은 확인 안 함)
- 잘못된 캐스팅 시 컴파일은 되지만 런타임 오류 가능

```cpp
CObj* pObj = new CPlayer;
static_cast<CPlayer*>(pObj)->CPlayer_Func();   // 올바른 사용
```

### dynamic_cast
- 런타임에 실제 타입 검사
- 잘못된 캐스팅 시 nullptr 반환

```cpp
CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
if (pPlayer)
    pPlayer->CPlayer_Func();
```

---

## 18. 상속에서 생성자/소멸자 호출 순서

**생성**: 부모 생성자 → 자식 생성자

**소멸**: 자식 소멸자 → 부모 소멸자

---

## 19. extern & friend

```cpp
extern int g_iA;   // 외부 파일에 존재하는 변수를 사용하겠다고 알림

friend class CEnemy;   // CEnemy가 내 private 멤버에 접근 가능 (은닉화 위반 주의)
```

---

## 20. 전방 선언

```cpp
// 헤더에서 전방 선언 가능한 경우
class B;
class A
{
    B* ptr;    // 포인터/참조 → 가능
    B& ref;
};

// 전방 선언만으로 불가능한 경우
class A
{
    B obj;           // 직접 멤버 → 불가능 (크기를 알아야 함)
    sizeof(B);
    new B();
};
```

> 보통 헤더에서는 전방 선언만 하고, cpp에서 실제 사용 시 해당 헤더를 include하는 방식을 사용.

---

## 21. 임시 객체

```cpp
INFO info;
INFO& a = info;          // 가능
const INFO& b = info;   // 가능

INFO& c = INFO();        // 불가능 (임시 객체는 곧 사라지므로)
const INFO& d = INFO();  // 가능 (const가 임시 객체의 수명을 연장)
```

---

## 22. 인라인 함수

매크로 함수의 장점(속도)을 취하고 단점을 보완한 함수.

```cpp
inline int Square(int _n)
{
    return _n * _n;
}
```

**인라인이 일반 함수로 바뀌는 경우**
- 함수 포인터로 사용할 경우
- 재귀 형태로 호출할 경우
- 컴파일러 판단

> 헤더 파일에 정의부를 구현해야 함.

---

## 23. 템플릿

하나의 틀에서 자료형이 다른 함수/클래스를 만들 수 있는 것.

```cpp
// 함수 템플릿
template <typename T>
T Add(T _a, T _b)
{
    return _a + _b;
}

cout << Add<int>(10, 20) << endl;    // int형 명시
cout << Add(10, 20) << endl;         // 자동 추론

// 클래스 템플릿
template <typename T>
class CObj
{
private:
    T m_A;
    T m_B;
};
CObj<int>   objInt;
CObj<float> objFloat;
```

> ⚠️ 템플릿은 파일 분할 불가 → 헤더에 선언과 정의를 모두 작성해야 함.

**템플릿 특수화**
```cpp
template<>
const char* Add(const char* _a, const char* _b)
{
    // 문자열 전용 처리
}
```

**템플릿 + static**
같은 자료형의 템플릿 클래스끼리만 static 멤버를 공유.

---

## 24. size_t

크기를 나타내기 위해 사용하는 부호 없는 정수형.

```cpp
for (size_t i = 0; i < vec.size(); ++i)   // 권장
// for (int i = 0; i < vec.size(); ++i)   // 경고 발생 가능
```

- 음수를 가질 수 없음 (unsigned)
- 32비트 환경 → 4바이트 / 64비트 환경 → 8바이트

---

## 25. STL (Standard Template Library)

### 컨테이너 종류

| 컨테이너 | 기반 | 탐색 | 삽입/삭제 | 특징 |
|---------|------|------|----------|------|
| vector | 배열 | 빠름 (인덱스) | 느림 (중간) | 동적 배열 |
| list | 노드 | 느림 (순회) | 빠름 | 더블 링크드 리스트 |
| map | 트리 | key로 접근 | 중간 | 자동 정렬, key-value |

### vector 주요 함수

```cpp
// 삽입
vec.push_back(val);              // 맨 뒤에 추가
vec.emplace_back(val);           // 맨 뒤에 직접 생성 (효율적)
vec.insert(vec.begin() + 2, val); // 중간 삽입

// 삭제
vec.pop_back();                  // 맨 뒤 삭제
vec.erase(vec.begin() + 2);     // 특정 위치 삭제
vec.clear();                     // 전체 삭제
vec.erase(remove(vec.begin(), vec.end(), val), vec.end()); // 값으로 삭제

// 조회
vec.size();      // 원소 개수
vec.capacity();  // 할당된 메모리 개수
vec.empty();     // 비어있으면 true
vec.front();     // 첫 번째 원소
vec.back();      // 마지막 원소
```

**vector 메모리 재할당 정책**

| push_back 후 원소 수 | capacity |
|---------------------|----------|
| 1 | 1 |
| 2 | 2 |
| 3 | 4 |
| 4 | 4 |
| 5 | 8 |

기존 capacity를 넘으면 `기존 크기 + (기존 크기 >> 1)` 만큼 재할당.

**메모리 예약**
```cpp
vec.reserve(10);   // 원소는 없고 capacity만 10으로 예약 (권장)
vector<int> v(10); // capacity 10 + 원소 10개 생성 (이후 clear() 필요)
```

### 반복자 (iterator)

```cpp
vector<int>::iterator iter = vec.begin();   // 첫 번째 원소
vector<int>::iterator iter = vec.end();     // 마지막 원소 다음

for (auto it = vec.begin(); it != vec.end(); ++it)
    cout << *it << endl;
```

| 반복자 종류 | 지원 연산 | 사용 컨테이너 |
|-----------|---------|------------|
| 임의 접근 | ++, --, [], +=, -= | vector |
| 양방향 | ++, -- | list, map |

### 알고리즘 함수 (`#include <algorithm>`)

```cpp
// 정렬
sort(vec.begin(), vec.end(), less<int>());     // 오름차순
sort(vec.begin(), vec.end(), greater<int>()); // 내림차순

// 조건 개수
int cnt = count_if(vec.begin(), vec.end(), OddNum<int>);

// 순회
for_each(vec.begin(), vec.end(), Safe_Delete<int*>);   // 동적할당 해제 용도로 자주 사용
```

---

## 26. 참조 반환 (const T&)

```cpp
const INFO& Get_Info() const
{
    return m_tInfo;   // 복사 없이 원본 참조 반환
}
```

- `T&` → 읽기/수정 가능한 참조 반환
- `const T&` → 읽기 전용 참조 반환 (복사 비용 없음, 최적화 목적)

---

## ✅ 부족한 부분 / 보완 추천

1. **스마트 포인터** (`unique_ptr`, `shared_ptr`) - 동적 메모리 관리를 더 안전하게
2. **람다(lambda)** - `for_each`, `sort` 등에서 함수 객체 대신 사용
3. **move semantics** - 불필요한 복사를 줄이는 현대 C++ 기법
4. **unordered_map** - map보다 탐색이 빠른 해시 기반 컨테이너 (게임에서 자주 사용)
5. **RAII 패턴** - 리소스 관리를 생성자/소멸자로 처리하는 패턴
