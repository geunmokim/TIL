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
        {
            pObj->Late_Update();

            RENDERID::ID eID = pObj->Get_RenderID();
            m_listRender[eID].emplace_back(pObj);
        }
    }
}

void CObjMgr::Render(HDC _DC)
{
    for (int i = 0; i < RENDERID::END; ++i)
    {
        if (RENDERID::OBJECT == i)
            m_listRender[i].sort(CompareY<CObj*>);  //OBJ들의 Y좌표값들을 비교해서 정렬 
                                                    //CompareY는 매개변수가 두개가 필요한데 업캐스팅으로 obj의 자식들을 m_listRender[i]에 해당하는 대로 불러옴
                                                   //불러온 자식들의 y좌표값들을 매개변수로 넣어서 값 비교
                                                   //왜 y좌표 정렬을 사용? -> 거리감을 결정하는 건 오직 Y좌표뿐이라서 
                                                   //정렬은 왜 필요한가? -> 가까운 게 먼 걸 가려야 한다
                                                   //화면 아래쪽(가까움)에 있는 오브젝트를, 화면 위쪽(멀음)에 있는 오브젝트보다 나중에 그려서 덮어써야한다
                                                   //실제로 x좌표 정렬을 하면 위/아래 가림 관계는 전혀 표현되지 않고, 대신 "좌우로 어느 게 화면 앞에 그려지는가"만 결정

        for (auto& pObj : m_listRender[i])
            pObj->Render(_DC);

        m_listRender[i].clear();
    }


    /*
    "종류(OBJID)별로 그리면 화면상 앞뒤 관계(레이어, 겹침 순서)를 표현할 수 없어서,
    '어느 레이어에 속하는가(RENDERID)'를 기준으로 먼저 큰 순서를 정하고,
    같은 레이어(OBJECT) 안에서는 Y좌표로 다시 세밀하게 정렬하는 방식으로 바꾼 것
    
    */
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