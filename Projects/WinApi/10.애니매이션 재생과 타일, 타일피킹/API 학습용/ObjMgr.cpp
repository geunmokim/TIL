#include "framework.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{

    

}
CObjMgr::~CObjMgr()
{
    Release();
}


void CObjMgr::Update()
{
    for (int i = 0; i < OBJID::END; ++i)
    {
        auto iter = m_listObj[i].begin();
        for (; iter != m_listObj[i].end();)
        {
            int iEvent = (*iter)->Update();
            if (OBJ_DEAD == iEvent)
            {
                SAFE_DELETE(*iter);
                iter = m_listObj[i].erase(iter);


            }
            else
                ++iter;

        }
    }
    CCollisionMgr::Collision_Rect(m_listObj[OBJID::MONSTER], m_listObj[OBJID::BULLET]);  
    //CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);  
    CCollisionMgr::Collision_Rect(m_listObj[OBJID::SHIELD], m_listObj[OBJID::MONSTER]);



}

void CObjMgr::Late_Update()
{
    for (int i = 0; i < OBJID::END; ++i)
    {
        for (auto& pObj : m_listObj[i])
            pObj->Late_Update();
    }
}

void CObjMgr::Render(HDC _DC)
{
    for (int i = 0; i < OBJID::END; ++i)
    {
        for (auto& pObj : m_listObj[i])
            pObj->Render(_DC);
    }
}

void CObjMgr::Release()
{
    for (int i = 0; i < OBJID::END; ++i)
    {
        for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
        m_listObj[i].clear();

    }
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
    for (auto& pObj : m_listObj[_eID])
        pObj->Set_bDead();
}

CObj* CObjMgr::Get_Target(OBJID::ID _eID, CObj* _pObj)
{
    if (m_listObj[_eID].empty())
        return nullptr;

    CObj* pTarget = nullptr;
    float fDis = 0.f;

    for (auto& pObj : m_listObj[_eID])
    {
        float fX = pObj->Get_Info().fX - _pObj->Get_Info().fX;
        float fY = pObj->Get_Info().fY - _pObj->Get_Info().fY;
        float fDia = sqrtf(fX * fX + fY * fY);

        if (fDis > fDia || !pTarget) //if(!pTarget || fDis > fDia)와 같다 //연산 우선 순위에 의해 fDis>fDia 먼저 실행하는 것 //!pTarget은 첫번쨰에만 true반환 나머지는 false
        {
            pTarget = pObj;
            fDis = fDia;

            /*
            첫 번째 객체는 무조건 선택                        //pTarget은 !pTarget -> !nullptr로 무조건 첫번쨰는 true가 나오므로
            그 이후에는 현재 선택된 객체보다 더 가까우면 교체
            하라는 코드 

             float fX = pObj->Get_Info().fX - _pObj->Get_Info().fX;
             float fY = pObj->Get_Info().fY - _pObj->Get_Info().fY;
             ->둘의 거리차이로 계산이므로 결국 불려오는 플레이어와 몬스터 사이의 거리 간격을 계산하는것

            */
        }


    }

    return pTarget;
}