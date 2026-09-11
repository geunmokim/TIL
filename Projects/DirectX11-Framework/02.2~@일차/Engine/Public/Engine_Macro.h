#pragma once


#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
}();

//싱글톤 인스턴스를 가져올때 , 참조 카운팅 (AddRef/ Release)규칙을 강제로 지키게 만들기 위한 안전장치
//[]() - 즉시 실행 람다
//매크로 안에 여러줄의 로직을 넣고 싶은데 그냥 #define으로 여러 문장을 넣으면
//나중에 다른 코드안에서 쓸 때 문법이 꺠지기 쉽다 
//->그래서 익명 함수를 만들어서 그 자리에서 바로 실행 하는 방식으로 감싸서
//이 매크로가 마치 하나의 값을 반환하는 단일 표현식처럼 안전하게 동작하게 만듦
//ex) CGraphic_Device* pDevice = GET_INSTANCE(CGraphic_Device)
//이렇게 쓰면 매크로가 통쨰로 함수 호출 하나 처럼 치환되어서 어느 문법에 넣어도 안꺠짐

//AddRef() 호출 하는 이유는 싱글톤도 참조 카운팅 규칙을 적용하기 위해서 


#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE {

#define END }

#ifdef ENGINE_EXPORTS /*엔진프로젝트라고 정의되어있다면. */
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


#define DECLARE_SINGLETON(ClassName)							\
public:															\
static ClassName* Get_Instance()								\
{																\
	if (!m_pInstance)											\
		m_pInstance = new ClassName;							\
	return m_pInstance;											\
}																\
static unsigned long Destroy_Instance()							\
{																\
	unsigned long		dwRefCnt = 0;							\
	if (nullptr != m_pInstance)									\
	{															\
		dwRefCnt = m_pInstance->Release();						\
		if(0 == dwRefCnt)										\
			m_pInstance = nullptr;								\
	}															\
	return dwRefCnt;											\
}																\
private:														\
	static ClassName* m_pInstance;								

#define IMPLEMENT_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr; 
