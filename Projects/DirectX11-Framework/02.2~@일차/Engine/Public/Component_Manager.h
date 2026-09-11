#pragma once

//#include "Shader.h"
//#include "Texture.h"
#include "Renderer.h"
//#include "Transform.h"
//#include "VIBuffer_Rect.h"
//#include "VIBuffer_Cube.h"
//#include "VIBuffer_Terrain.h"


BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	//레벨 개수만큼 원형창고(map 배열)를 미리 확보
	HRESULT Reserve_Manager(_uint iNumLevels);

	//특정 레벨의 창고에 pPrototypeTag로 pPrototype을 등록해라 
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);

	//특정 레벨 창고에서 pPrototypeTag로 원형을 찾아서 복제해서 반환
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

	//레벨이 끝나거나 전환될떄 그 레벨의 원형들을 정리
	HRESULT Clear_Component(_uint iLevelIndex);

private:
	map<const _tchar*, class CComponent*>* m_pPrototypes = nullptr; 
	//map을 하나만 쓸거면 포인터로 사용할 필요가 없는데 몇개가 사용될지 미리 정해지지 않고 실행중에 받는걸 동적으로 배열을 만들어야해서 포인터 사용 
	//m_pPrototypes -> 레벨별로 나뉜 원형 창고
	/*
	m_pPrototypes[0] → 0번 레벨의 원형 창고 (map)
    m_pPrototypes[1] → 1번 레벨의 원형 창고 (map)
    m_pPrototypes[2] → 2번 레벨의 원형 창고 (map)
	레벨 별로 원형을 관리하는 이유 -> 숲 레벨에는 늑대몬스터만, 동굴 레벨에는 박쥐 몬스터만 필요하다면 안쓰는 레벨의 원형까지 메모리에 들고 있을 필요는 없으므로 
	레벨이 전환되면 그 레벨의 원형창고만 정리(clear_component)하는 구조 

	*/

	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	_uint				m_iNumLevels = 0;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END