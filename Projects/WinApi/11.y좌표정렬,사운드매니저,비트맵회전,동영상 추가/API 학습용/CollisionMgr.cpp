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

	for (auto& pDst : _Dst)     
	{
		for (auto& pSrc : _Src)
		{
			if (IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect()))
			{
				pDst->Set_bDead(); 
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
	float	fX = abs(_pDst->Get_Info().fX - _pSrc->Get_Info().fX); 
	float	fY = _pDst->Get_Info().fY - _pSrc->Get_Info().fY;      
	float	fDia = sqrtf(fX * fX + fY * fY);                       

	float	fSumRad = (float)((_pDst->Get_Info().iCX + _pSrc->Get_Info().iCX) >> 1);

	
	if (fSumRad >= fDia)
		return true;

	return false;
}

void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{

}