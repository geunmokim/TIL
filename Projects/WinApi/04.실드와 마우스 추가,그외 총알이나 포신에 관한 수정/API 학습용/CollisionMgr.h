#pragma once
class CObj;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	//static 함수이므로 객체 생성 없이 호출하겠다
	//list<CObj*>& -> CObj*형으로 만든 list를 참조로 받음(복사 x)
	//참조(&)로 받는 이유가 뭘까? 
	//->
	/*
	// 참조 없이 받으면
        void Collision_Rect(list<CObj*> _Dst)
     → list 전체를 복사 → 총알, 몬스터 많으면 성능 낭비

     // 참조로 받으면
     void Collision_Rect(list<CObj*>& _Dst)
     → 원본 주소만 전달 → 복사 없음 → 성능 좋음
	*/
	
	static void Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src);

	static bool Check_Sphere(CObj* _pDst, CObj* _pSrc);
};

