#pragma once
#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext); //explicit -> 암묵적 형변환을 막기 위해 사용하는데 생성자가 매개변수를 가질 경우 붙인다 
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;     //default -> 기본 동작 그대로 써라 라고 컴파일러에게 명시적으로 알림 

public:
	class CComponent* Get_Components(const _tchar* pComponentTag);  //컴포넌트 창고 관리

public:
	//게임 오브젝트의 생애주기(lifecycle)을 정의하는 표준 인터페이스
	virtual HRESULT NativeConstruct_Prototype(); //원형을 만들때 호출(원형은 실제로 화면에 안나오고, 복제의 원본으로만 쓰이는 개념)

	virtual HRESULT NativeConstruct(void* pArg); //원형에서 복제된 실제 인스턴스를 초기화할때 호출  //pArg는 초기화에 필요한 가변적 데이터(위치, 초기값등)를 넘기는 범용 매개변수 

	virtual _int Tick(_double TimeDelta); //매 프레임 로직 갱신(델타타임). -이동,상태 갱신등 
	//timer에서 델타타임을 다루는데 tick이 필요한 이유 -> timer는 델타타임을 계산만 하고 tick은 누군가 넘겨준 그값을 전달받아서 활용하는 곳 

	virtual _int LateTick(_double TimeDelta); //tick 이후에 한번더 갱신(카메라가 캐릭터가 다 움직인 후에 카메라 위치 잡기 등등을 위해 필요)

	virtual HRESULT Render();
protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	map<const _tchar*, class CComponent*>				m_Components;
	typedef map<const _tchar*, class CComponent*>		COMPONENTS;



protected:
	//컴포넌트를 오브젝트에 붙이는 함수 
	virtual HRESULT SetUp_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);




private:
	CComponent* Find_Component(const _tchar* pComponentTag);    //컴포넌트 창고 관리 


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END