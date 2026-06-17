#include "framework.h"
#include "CollisionMgr.h"
#include "Obj.h"

CCollisionMgr::CCollisionMgr()
{

}


CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	for (auto& pDst : _Dst)      //CObj*형으로 만들어진 list를 전부 순회하며
	{
		for (auto& pSrc : _Src)
		{
			if (IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect())) //&rc-> 겹친 영역을 저장할 변수,첫번쨰rect,두번쨰rect //-> 겹치면 true반환
			{
				pDst->Set_bDead(); //Set_bDead()는 true를 반환하므로 영역이 겹쳤다는 판단이 들면 해당 object가 죽는다
				pSrc->Set_bDead();
			}
		}
	}
}
void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& pDst : _Dst)
	{
		for (auto& pSrc : _Src)
		{
			if (Check_Sphere(pDst, pSrc))
			{
				pDst->Set_bDead();
				pSrc->Set_bDead();
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* _pDst, CObj* _pSrc)
{
	float	fX = abs(_pDst->Get_Info().fX - _pSrc->Get_Info().fX); //두 오브젝트의 x거리 //abs()->#include <cstdlib>에 포함// 절댓값 반환, 음수면 양수로 바꿔줌
	float	fY = _pDst->Get_Info().fY - _pSrc->Get_Info().fY;      //두 오브젝트의 y거리
	float	fDia = sqrtf(fX * fX + fY * fY);                       //두 오브젝트 사이의 실제 거리 //sqrtf()-> #include <cmath>에 포함// 제곱근 반환, sqrt의 float 버전
	/*
	
	        fY
         |  /
         | / fDia (실제 거리)
         |/
---------+--------- fX
	
	fX² + fY² = fDia² -> 피타고라스의 정리
	*/

	
	// 두 오브젝트 CX(지름) 합 / 2 = 반지름 합
	float	fSumRad = (float)((_pDst->Get_Info().iCX + _pSrc->Get_Info().iCX) >> 1);

	
	if (fSumRad >= fDia)
		return true;
	/*
	두 오브젝트 사이 거리(fDia) < 반지름 합(fSumRad)
		→ 겹쳤다 = 충돌!
	*/
	return false;
}
