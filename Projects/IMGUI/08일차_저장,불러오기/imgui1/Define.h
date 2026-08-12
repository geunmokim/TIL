#pragma once

//그래픽 디바이스는 프로그램에 딱 하나만 있어야 하니까 싱클톤으로 관리
//\를 붙여줘야 매크로가 다음줄까지 이어진다는것을 알려줌
#define DECLARE_SINGLETON(ClassName)					\
public:													\
	static ClassName* Get_Instance();					\
	static void Destroy_Instance(void);				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)						\
		m_pInstance = new ClassName;					\
	return m_pInstance;								\
}														\
void ClassName::Destroy_Instance(void) {				\
	if (nullptr != m_pInstance) {						\
		delete m_pInstance;							\
		m_pInstance = nullptr;							\
	}													\
}

#define ERR_MSG(message)	::MessageBoxW(nullptr, message, L"error", MB_OK)

/*매크로를 두개로 쪼개놓은 이유는
이전엥는 , Get_Instance(), Destroy_Instance()의 몸통까지 전부 헤더안에 넣어놨었는데 
이렇게 다들어간 헤더를 여러 cpp에서 include 할 경우 문제가 생길 수 있었다 
정확히는 Get_instance() 몸통이 헤더 안에 그대로 있으면 여러 cpp에서 include 할 경우 
똑같은 함수 몸통이 두번 컴파일 되면서 이미 정의된 함수다 라고 링커가 에러를 낼 수 있다(inline을 붙이면 해결되긴함)
그러한 것을 매크로를 두개로 쪼개놓은것에서 이득 볼 수 있다
  */