#pragma once
#include "Obj.h"

// 화살을 목표 칸 위치에 그대로 배치하고 단 한 프레임만 보여준 뒤 사라지는 방식으로 구현한다.
//->히트스캔방식
class CArrow : public CObj
{

public:
	CArrow();
	virtual ~CArrow();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public:
	// Initialize()는 Obj의 시그니처(매개변수 없음)를 따라야 하므로 위치를 받을 수 없다.
	// 그래서 화살이 나타날 칸 위치는 생성 직후 이 함수로 별도 지정한다.
	void Set_TargetPos(const GRIDPOS& _targetPos)   // 따로 만든 함수로 위치를 받음
	{
		Set_GridPos(_targetPos.row, _targetPos.col);
	}
private:
	// ObjMgr::Update()는 Update()가 OBJ_DEAD를 반환하는 즉시 그 프레임에서 객체를 삭제한다.
	// 그러면 그 뒤에 이어지는 Late_Update()/Render()가 호출되지 못해 화면에 전혀 안 보이게 된다.
	// 그래서 "이번 프레임은 살아서 Render까지 호출되고, 그 다음 프레임에 죽는다"는 한 프레임의
	// 유예를 두기 위해 카운터를 사용한다.
	int m_iLifeFrame;
};

