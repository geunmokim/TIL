#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext); //ID3D11Device,ID3D11DeviceContext는 dx11 그래픽작업에서 자원생성, 그리기에 필수로 요하기 떄문에 
	                                                                                   //부모생성자에 미리 매개변수로 넣어두고 상속 받는 자식이 매번 일일이 호출할 필요없이 사용하게끔 만든다

	explicit CComponent(const CComponent& rhs);   //복사 생성자 //&(참조)가 아니면 그자체가 복사를 한번 더 유발해서 무한 재귀로 에러 발생-> 원본객체를 복사하지말고 가리키기만 하라고 참조 사용 
	                                              //const로 읽기 전용으로만 사용하라고 강제해서 실수로 원본을 고치는 코드를 사용하면 컴파일 에러가 발생하게함 
	                                              //복사 생성자가 필요한 이유는 원형 패턴(prototype)을 다루기 때문 
	                                              //게임 안에서 몬스터가 수백마리를 필요로 할때 매번 new monster로 만들면 수백번 반복해서 성능이 크게 나뻐진다 
	                                              //->원형을 만들어두고 몬스터가 필요할떄마다 복제만 해서 사용 //복제는 이미 로드된 데이터를 그대로 복사하는것이라 파일을 다시 읽을 필요가 없어서 훨씬 빠름



	virtual ~CComponent() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(); //원형을 만들때 호출되는 초기화 함수 
	virtual HRESULT NativeConstruct(void* pArg); //원형에서 복제된 실제 인스턴스를 초기화할떄 호출 


protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	_bool						m_isCloned = false;   //원형인지 복제인지 구분 //원형,복제가 수행할 일이 다르기 떄문 

public:
	virtual CComponent* Clone(void* pArg) = 0; //자식들에게 복제해라 라는 규칙을 강제 
	virtual void Free() override; //삭제되기 전에 이 클래스가 소유한 자원들을 정리하는 역할 
};

END

/*
 1.Clone(void* pArg) = 0 — "복제해라"라는 규칙을 강제
 2.explicit CComponent(const CComponent& rhs) — 그 복제를 실제로 수행하는 수단 (복사 생성자)
 3.NativeConstruct(void* pArg) — 복제 직후, 이 인스턴스만의 세부 초기화
 이 세가지가 한세트로 묶여서 원형-복제 패턴 완성 

*/