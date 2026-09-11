#include "..\public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

HRESULT CRenderer::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_RenderGroup[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderGroup[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject); //여기서 release()해도 render()한것이 지워지지는 않음 -> render()한것이 다른곳에서도 사용되어서 그곳에서도 참조 카운팅이 유지되고 있기 떄문 

	}

	m_RenderGroup[RENDER_PRIORITY].clear();
	//clear로 비우지 않으면 새로 그릴떄 이전의 것들이 남아서 계속 그려지는 버그가 생김


	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{
	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroup[RENDER_NONALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	//반투명 그룹만 정렬이 필요한 이유 
	// 왜?
	// -> 반투명 물체는 카메라에서 먼 것부터 가까운 순서로 그려야 제대로 겹쳐 보인다 그래서 먼 것이 앞에 오도록 정렬한 뒤에 그린다
	
	//m_RenderGroup[RENDER_ALPHA].sort([&](CGameObject* pSour, CGameObject* pDest) 
	//{
	//	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
	//});

	for (auto& pGameObject : m_RenderGroup[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroup[RENDER_ALPHA].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderGroup[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderGroup[RENDER_UI].clear();

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free(); //부모(CComponent)몫:m_pDevice, m_pDeviceContext 정리

	for (auto& RenderList : m_RenderGroup) //자식 몫: 자기가 가진 오브젝트 리스트 정리 
	{
		for (auto& pGameObject : RenderList)
			Safe_Release(pGameObject);

		RenderList.clear();
	}
}
/*
C++의 표준 원칙: 생성은 부모→자식, 소멸은 자식→부모

생성 (Constructor): 부모 먼저 → 자식 나중
소멸 (Destructor):   자식 먼저 → 부모 나중
인데 free()는 내가 만든 일반 함수라서 부모를 먼저 부를지 자식을 먼저 부를지는 개발자가 정할 수 있다
그럼 왜 부모를 먼저 부르도록 설계했을까?>
->각자 자기가 가진 자원만 책임지고 정리한다는 원칙을 지키면서 정리 순서를 신경 쓸 필요 없게 만들기 위해서



*/
