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
};
/*
만약 팩토리 없이 쓴다면
// 총알 만들 때마다 이걸 반복
CObj* pObj = new CBullet;
pObj->Initialize();
pObj->Set_Pos(x, y);

// 몬스터 만들 때마다 이걸 반복
CObj* pObj = new CMonster;
pObj->Initialize();
pObj->Set_Pos(x, y);

팩토리 쓰면

// 총알
CAbFactory<CBullet>::Create(x, y);

// 몬스터
CAbFactory<CMonster>::Create(x, y);
한 줄로 끝
->template 덕분에 호출할 때 한 줄로 끝남
*/