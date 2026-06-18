#pragma once
class CObj;
template <typename T>
class CAbFactory
{
public:
	static CObj* Create()                         //static으로 함수 선언 했으므로 객체생성 없이 호출  가능  CObj*는 이 함수가 반환하는 타입
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj; //포인터를 반환

	}

	static CObj* Create(float _x, float _y)  //마찬가지로 static으로 함수 선언 했으므로  객체 생성 없이 
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);
		return pObj;

	}
	static CObj* Create(float _x, float _y, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_fAngle);

		return pObj;

	}
	static CObj* Create(CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Target(_pTarget);

		return pObj;

	}
};
