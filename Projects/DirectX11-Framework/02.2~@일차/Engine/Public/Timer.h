#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;
public:
	HRESULT NativeConstruct();
	_float Compute_TimeDelta();
private:
	LARGE_INTEGER		m_CurrentTime;
	LARGE_INTEGER		m_OldTime;
	LARGE_INTEGER		m_OriginTime;
	LARGE_INTEGER		m_CpuTick;
	double				m_TimeDelta;
public:
	static CTimer* Create();
	virtual void Free() override;
};

END

/* 타임델타를 구해준다. */
//타임 델타 -> 유저가 플레이할떄 플레이가 유저 컴퓨터 사양에 영향을 받지 않고 개발자가 설계한 대로 움직일 수 있게끔 보정 해줌 
//CTimer는 델타타임을 실제로 계산하는 로직 하나. 시계하나라고 생각하면 되고 timer_manger가 시계 여러개를 관리 하는 것