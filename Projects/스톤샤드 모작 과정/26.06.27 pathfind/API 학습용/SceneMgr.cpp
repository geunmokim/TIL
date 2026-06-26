#include "framework.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Menu.h"
#include "Stage.h"
#include "Edit.h"
CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_ePreScene(END), m_eCurScene(END)
{

}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID _eID)
{
	m_eCurScene = _eID;

	if (m_ePreScene != m_eCurScene) //만약 전의 씬이 현재의 씬과 다르다면
	{
		Release();                 //전의 씬의 객체를 삭제해라


		switch (m_eCurScene)
		{
		case CSceneMgr::LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneMgr::MENU:
			m_pScene = new CMyMenu;
			break;
		case CSceneMgr::STAGE:  
			m_pScene = new CStage;
			break;
		case CSceneMgr::EDIT:  
			m_pScene = new CMyEdit;
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}
void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
}