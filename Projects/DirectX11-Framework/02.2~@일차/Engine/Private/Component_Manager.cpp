#include "..\public\Component_Manager.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pPrototypes = new PROTOTYPES[iNumLevels]; //new [...]로 레벨 개수만큼 map 배열을 동적 할당

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pPrototypes ||
		iLevelIndex >= m_iNumLevels ||
		nullptr == pPrototype ||
		nullptr != Find_Prototype(iLevelIndex, pPrototypeTag)) //이미 같은 태그로 등록된 원형이 있다면 등록하지 마라
		return E_FAIL;

	m_pPrototypes[iLevelIndex].insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));
	
	return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototypes ||
		iLevelIndex >= m_iNumLevels)
		return nullptr;

	CComponent* pPrototype = Find_Prototype(iLevelIndex, pPrototypeTag);  //Find_Prototype으로 찾고
	if (nullptr == pPrototype)
		return nullptr;

	CComponent* pComponent = pPrototype->Clone(pArg);  //찾은 원형의 Clone(pArg) 호출 
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;//clone 반환  ---> 찾아서 복제 후 반환 사이클 
}

HRESULT CComponent_Manager::Clear_Component(_uint iLevelIndex)
{
	if (iLevelIndex > m_iNumLevels)
		return E_FAIL;

	for (auto& Pair : m_pPrototypes[iLevelIndex])
		Safe_Release(Pair.second);      //map의 second 즉 value값을 정리

	m_pPrototypes[iLevelIndex].clear();

	return S_OK;
}

CComponent* CComponent_Manager::Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag)
{
	auto	iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;          
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);

		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pPrototypes);    //new PROTOTYPES[iNumLevels]로 배열 할당했으니 정리할때도 배열로 정리
}
