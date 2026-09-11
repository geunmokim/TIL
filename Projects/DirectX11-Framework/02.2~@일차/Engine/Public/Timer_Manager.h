#pragma once

#include "Base.h"

/* 타이머를 여러개 생성하여 보관한다.  */
//게임 안에는 서로 다른 흐름으로 시간을 재야 하는 상황들이 발생해서 매니저 생성
//슬로우 모션, 디버그/에디터 용도, 일시정지 등등 


BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
public:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);

private: /* 생성한 타이머를 보관하기위한 컨테이너. */
	map<const _tchar*, class CTimer*>			m_Timers;
	typedef map<const _tchar*, class CTimer*>	TIMERS;
private:
	class CTimer* Find_Timer(const _tchar* pTimerTag);

public:
	virtual void Free() override;
};

END