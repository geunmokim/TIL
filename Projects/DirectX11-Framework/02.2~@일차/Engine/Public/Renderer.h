#pragma once

/* 화면에 그려져야할 객체들을 모아서 보관한다. */
/* 보관할때 그려져야하는 순서대로 출력할수있도록 나눠서 보관하낟. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDER { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
	/*
	RENDER_PRIORITY: 뭔가 최우선으로 먼저 그려야 하는 것 (예: 하늘, 배경, 지형처럼 가장 뒤에 깔려야 하는 것들)
	RENDER_NONALPHA: 불투명한 오브젝트들(몬스터, 캐릭터, 건물 등등 알파 블렌딩 없이 그냥 
	RENDER_ALPHA:반투명 오브젝트들(유리, 이펙트, 물 등 - 뒤에서부터 순서대로 그려야 정확하게 겹쳐 보임
	RENDER_UI:UI(체력바,인벤토리 등)- 항상 다른 3D 오브젝트들보다 맨 앞(화면 위)에 그려야 함
	
	*/
private:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;
public:
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pGameObject); //오브젝트를 오늘 그릴 목록에 등록 
	//매프레임마다 render()가 실행되는게 아니라여기 등록해두는 방식

	HRESULT Draw_RenderGroup();
private:
	//핵심 자료 구조
	list<class CGameObject*>				m_RenderGroup[RENDER_END];
	//RENDER_END(4) 크기의 배열인데, 각 칸이 list<CGameObject*>로 이루어짐
	/*
	m_RenderGroup[RENDER_PRIORITY]  → 우선 그릴 오브젝트들의 리스트
    m_RenderGroup[RENDER_NONALPHA]  → 불투명 오브젝트들의 리스트
    m_RenderGroup[RENDER_ALPHA]     → 반투명 오브젝트들의 리스트
    m_RenderGroup[RENDER_UI]        → UI 오브젝트들의 리스트

	이런식으로 4개의 리스트를 한번에 관리하는 구조 
	*/


	typedef list<class CGameObject*>		RENDERGROUP;
public:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

/*
CRenderer가 정해진 순서(우선 → 불투명 → 반투명 → UI)대로 한꺼번에 정리해서 그려준다
*/